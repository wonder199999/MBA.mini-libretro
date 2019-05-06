/* ======================================================================== */
/* ================================ INCLUDES ============================== */
/* ======================================================================== */

#include "emu.h"
#include "debugger.h"
#include <setjmp.h>
#include "m68kcpu.h"
#include "m68kops.h"

/* ======================================================================== */
/* ================================= DATA ================================= */
/* ======================================================================== */

/* Number of clock cycles to use for exception processing.
 * I used 4 for any vectors that are undocumented for processing times.
 */
static const UINT8 m68ki_exception_cycle_table[256] = {

		 40, /*  0: Reset - Initial Stack Pointer                      */
		  4, /*  1: Reset - Initial Program Counter                    */
		 50, /*  2: Bus Error                             (unemulated) */
		 50, /*  3: Address Error                         (unemulated) */
		 34, /*  4: Illegal Instruction                                */
		 38, /*  5: Divide by Zero                                     */
		 40, /*  6: CHK                                                */
		 34, /*  7: TRAPV                                              */
		 34, /*  8: Privilege Violation                                */
		 34, /*  9: Trace                                              */
		  4, /* 10: 1010                                               */
		  4, /* 11: 1111                                               */
		  4, /* 12: RESERVED                                           */
		  4, /* 13: Coprocessor Protocol Violation        (unemulated) */
		  4, /* 14: Format Error                                       */
		 44, /* 15: Uninitialized Interrupt                            */
		  4, /* 16: RESERVED                                           */
		  4, /* 17: RESERVED                                           */
		  4, /* 18: RESERVED                                           */
		  4, /* 19: RESERVED                                           */
		  4, /* 20: RESERVED                                           */
		  4, /* 21: RESERVED                                           */
		  4, /* 22: RESERVED                                           */
		  4, /* 23: RESERVED                                           */
		 44, /* 24: Spurious Interrupt                                 */
		 44, /* 25: Level 1 Interrupt Autovector                       */
		 44, /* 26: Level 2 Interrupt Autovector                       */
		 44, /* 27: Level 3 Interrupt Autovector                       */
		 44, /* 28: Level 4 Interrupt Autovector                       */
		 44, /* 29: Level 5 Interrupt Autovector                       */
		 44, /* 30: Level 6 Interrupt Autovector                       */
		 44, /* 31: Level 7 Interrupt Autovector                       */
		 34, /* 32: TRAP #0                                            */
		 34, /* 33: TRAP #1                                            */
		 34, /* 34: TRAP #2                                            */
		 34, /* 35: TRAP #3                                            */
		 34, /* 36: TRAP #4                                            */
		 34, /* 37: TRAP #5                                            */
		 34, /* 38: TRAP #6                                            */
		 34, /* 39: TRAP #7                                            */
		 34, /* 40: TRAP #8                                            */
		 34, /* 41: TRAP #9                                            */
		 34, /* 42: TRAP #10                                           */
		 34, /* 43: TRAP #11                                           */
		 34, /* 44: TRAP #12                                           */
		 34, /* 45: TRAP #13                                           */
		 34, /* 46: TRAP #14                                           */
		 34, /* 47: TRAP #15                                           */
		  4, /* 48: FP Branch or Set on Unknown Condition (unemulated) */
		  4, /* 49: FP Inexact Result                     (unemulated) */
		  4, /* 50: FP Divide by Zero                     (unemulated) */
		  4, /* 51: FP Underflow                          (unemulated) */
		  4, /* 52: FP Operand Error                      (unemulated) */
		  4, /* 53: FP Overflow                           (unemulated) */
		  4, /* 54: FP Signaling NAN                      (unemulated) */
		  4, /* 55: FP Unimplemented Data Type            (unemulated) */
		  4, /* 56: MMU Configuration Error               (unemulated) */
		  4, /* 57: MMU Illegal Operation Error           (unemulated) */
		  4, /* 58: MMU Access Level Violation Error      (unemulated) */
		  4, /* 59: RESERVED                                           */
		  4, /* 60: RESERVED                                           */
		  4, /* 61: RESERVED                                           */
		  4, /* 62: RESERVED                                           */
		  4, /* 63: RESERVED                                           */
		     /* 64-255: User Defined                                   */
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
		  4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};


/********************************************************/
/*	MEMORY READ/WRITE CALLS				*/
/********************************************************/

static UINT8 memory_read_byte(address_space *space, offs_t address)
{
	return space->read_byte(address);
}

static UINT16 memory_read_word(address_space *space, offs_t address)
{
	return space->read_word(address);
}

static UINT32 memory_read_dword(address_space *space, offs_t address)
{
	return space->read_dword(address);
}

static void memory_write_byte(address_space *space, offs_t address, UINT8 data)
{
	space->write_byte(address, data);
}

static void memory_write_word(address_space *space, offs_t address, UINT16 data)
{
	space->write_word(address, data);
}

static void memory_write_dword(address_space *space, offs_t address, UINT32 data)
{
	space->write_dword(address, data);
}


/***************************************************************************
    CPU STATE DESCRIPTION
***************************************************************************/

INLINE m68ki_cpu_core *get_safe_token(running_device *device)
{
	assert(device != NULL);
	assert(device->type() == M68000 || device->type() == M68008);

	return (m68ki_cpu_core *)downcast<legacy_cpu_device *>(device)->token();
}


/* ======================================================================== */
/* ================================= API ================================== */
/* ======================================================================== */

static void set_irq_line(m68ki_cpu_core *m68k, int irqline, int state)
{
	UINT32 old_level = m68k->int_level;
	UINT32 vstate = m68k->virq_state;
	UINT32 blevel;

	if (state == ASSERT_LINE)
		vstate |= 1 << irqline;
	else
		vstate &= ~(1 << irqline);
	m68k->virq_state = vstate;

	for (blevel = 7; blevel > 0; blevel--)
		if (vstate & (1 << blevel))
			break;

	m68k->int_level = blevel << 8;

	/* A transition from < 7 to 7 always interrupts (NMI) */
	/* Note: Level 7 can also level trigger like a normal IRQ */
	if (old_level != 0x0700 && m68k->int_level == 0x0700)
		m68k->nmi_pending = TRUE;
}

static void m68k_presave(running_machine *machine, void *param)
{
	m68ki_cpu_core *m68k = (m68ki_cpu_core *)param;
	m68k->save_sr = m68ki_get_sr(m68k);
	m68k->save_stopped = (m68k->stopped & STOP_LEVEL_STOP) != 0;
	m68k->save_halted  = (m68k->stopped & STOP_LEVEL_HALT) != 0;
}

static void m68k_postload(running_machine *machine, void *param)
{
	m68ki_cpu_core *m68k = (m68ki_cpu_core *)param;
	m68ki_set_sr_noint_nosp(m68k, m68k->save_sr);
	m68k->stopped = m68k->save_stopped ? STOP_LEVEL_STOP : 0 | m68k->save_halted  ? STOP_LEVEL_HALT : 0;
	m68ki_jump(m68k, REG_PC);
}

/* global access */

void m68k_set_encrypted_opcode_range(running_device *device, offs_t start, offs_t end)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	m68k->encrypted_start = start;
	m68k->encrypted_end = end;
}

void m68k_set_reset_callback(running_device *device, m68k_reset_func callback)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	m68k->reset_instr_callback = callback;
}

void m68k_set_cmpild_callback(running_device *device, m68k_cmpild_func callback)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	m68k->cmpild_instr_callback = callback;
}

void m68k_set_rte_callback(running_device *device, m68k_rte_func callback)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	m68k->rte_instr_callback = callback;
}

void m68k_set_tas_callback(running_device *device, m68k_tas_func callback)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	m68k->tas_instr_callback = callback;
}

/* translate logical to physical addresses */
static CPU_TRANSLATE( m68k )
{
	return TRUE;
}

/* Execute some instructions until we use up cycles clock cycles */
static CPU_EXECUTE( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	m68k->initial_cycles = m68k->remaining_cycles;

	/* eat up any reset cycles */
	if (m68k->reset_cycles)
	{
		int rc = m68k->reset_cycles;
		m68k->reset_cycles = 0;
		m68k->remaining_cycles -= rc;

		if (m68k->remaining_cycles <= 0)
			return;
	}

	/* See if interrupts came in */
	m68ki_check_interrupts(m68k);

	/* Make sure we're not stopped */
	if (!m68k->stopped)
	{
		/* Return point if we had an address error */
		m68ki_set_address_error_trap(m68k); /* auto-disable (see m68kcpu.h) */

		/* Main loop.  Keep going until we run out of clock cycles */
		do
		{
			/* Set tracing accodring to T1. (T0 is done inside instruction) */
			m68ki_trace_t1(); /* auto-disable (see m68kcpu.h) */

			/* Call external hook to peek at CPU */
			debugger_instruction_hook(device, REG_PC);

			/* Record previous program counter */
			REG_PPC = REG_PC;

			/* Read an instruction and call its handler */
			m68k->ir = m68ki_read_imm_16(m68k);
			m68ki_instruction_jump_table[m68k->ir](m68k);
			m68k->remaining_cycles -= m68k->cyc_instruction[m68k->ir];

			/* Trace m68k_exception, if necessary */
			m68ki_exception_if_trace(); /* auto-disable (see m68kcpu.h) */
		}
		while (m68k->remaining_cycles > 0);

		/* set previous PC to current PC for the next entry into the loop */
		REG_PPC = REG_PC;
	}
	else if (m68k->remaining_cycles > 0)
		m68k->remaining_cycles = 0;
}

static CPU_INIT( m68k )
{
	static UINT32 emulation_initialized = 0;
	m68ki_cpu_core *m68k = get_safe_token(device);

	m68k->device = device;
	m68k->program = device->space(AS_PROGRAM);
	m68k->int_ack_callback = irqcallback;

	/* The first call to this function initializes the opcode handler jump table */
	if (!emulation_initialized)
	{
		m68ki_build_opcode_table();
		emulation_initialized = 1;
	}

	/* Note, D covers A because the dar array is common, REG_A=REG_D+8 */
	state_save_register_device_item_array(device, 0, REG_D);
	state_save_register_device_item(device, 0, REG_PPC);
	state_save_register_device_item(device, 0, REG_PC);
	state_save_register_device_item(device, 0, REG_USP);
	state_save_register_device_item(device, 0, REG_ISP);
	state_save_register_device_item(device, 0, REG_MSP);
	state_save_register_device_item(device, 0, m68k->vbr);
	state_save_register_device_item(device, 0, m68k->save_sr);
	state_save_register_device_item(device, 0, m68k->int_level);
	state_save_register_device_item(device, 0, m68k->save_stopped);
	state_save_register_device_item(device, 0, m68k->save_halted);
	state_save_register_device_item(device, 0, m68k->pref_addr);
	state_save_register_device_item(device, 0, m68k->pref_data);
	state_save_register_presave(device->machine, m68k_presave, m68k);
	state_save_register_postload(device->machine, m68k_postload, m68k);
}

/* Pulse the RESET line on the CPU */
static CPU_RESET( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	/* Clear all stop levels and eat up all remaining cycles */
	m68k->stopped = 0;
	if (m68k->remaining_cycles > 0)
		m68k->remaining_cycles = 0;

	m68k->run_mode = RUN_MODE_BERR_AERR_RESET;

	/* Turn off tracing */
	m68k->t1_flag = m68k->t0_flag = 0;
	m68ki_clear_trace();

	/* Interrupt mask to level 7 */
	m68k->int_mask = 0x0700;
	m68k->int_level = m68k->virq_state = 0;

	/* Reset VBR */
	m68k->vbr = 0;

	/* Go to supervisor mode */
	m68ki_set_sm_flag(m68k, SFLAG_SET | MFLAG_CLEAR);

	/* Invalidate the prefetch queue */
	/* Set to arbitrary number since our first fetch is from 0 */
	m68k->pref_addr = 0x1000;

	/* Read the initial stack pointer and program counter */
	m68ki_jump(m68k, 0);
	REG_SP = m68ki_read_imm_32(m68k);
	REG_PC = m68ki_read_imm_32(m68k);
	m68ki_jump(m68k, REG_PC);

	m68k->run_mode = RUN_MODE_NORMAL;

	m68k->reset_cycles = m68k->cyc_exception[EXCEPTION_RESET];
}

static CPU_DISASSEMBLE( m68k )
{
	return 0x0001;
}


/**************************************************************************
 * STATE IMPORT/EXPORT
 **************************************************************************/

static CPU_IMPORT_STATE( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	switch (entry.index())
	{
		case M68K_SR:
		case STATE_GENFLAGS:
			m68ki_set_sr(m68k, m68k->iotemp);
			break;

		case M68K_ISP:
			if (m68k->s_flag && !m68k->m_flag)
				REG_SP = m68k->iotemp;
			else
				REG_ISP = m68k->iotemp;
			break;

		case M68K_USP:
			if (!m68k->s_flag)
				REG_SP = m68k->iotemp;
			else
				REG_USP = m68k->iotemp;
			break;

		case M68K_MSP:
			if (m68k->s_flag && m68k->m_flag)
				REG_SP = m68k->iotemp;
			else
				REG_MSP = m68k->iotemp;
			break;

		default:
			fatalerror("CPU_IMPORT_STATE(m68k) called for unexpected value\n");
			break;
	}
}


static CPU_EXPORT_STATE( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	switch (entry.index())
	{
		case M68K_SR:
		case STATE_GENFLAGS:
			m68k->iotemp = m68ki_get_sr(m68k);
			break;

		case M68K_ISP:
			m68k->iotemp = (m68k->s_flag && !m68k->m_flag) ? REG_SP : REG_ISP;
			break;

		case M68K_USP:
			m68k->iotemp = (!m68k->s_flag) ? REG_SP : REG_USP;
			break;

		case M68K_MSP:
			m68k->iotemp = (m68k->s_flag && m68k->m_flag) ? REG_SP : REG_MSP;
			break;

		case M68K_FP0:
		case M68K_FP1:
		case M68K_FP2:
		case M68K_FP3:
		case M68K_FP4:
		case M68K_FP5:
		case M68K_FP6:
		case M68K_FP7:
			break;

		default:
			fatalerror("CPU_EXPORT_STATE(m68k) called for unexpected value\n");
			break;
	}
}

static CPU_SET_INFO( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	switch (state)
	{
		/* --- the following bits of info are set as 64-bit signed integers --- */
		case CPUINFO_INT_INPUT_STATE + 0:
		case CPUINFO_INT_INPUT_STATE + 1:
		case CPUINFO_INT_INPUT_STATE + 2:
		case CPUINFO_INT_INPUT_STATE + 3:
		case CPUINFO_INT_INPUT_STATE + 4:
		case CPUINFO_INT_INPUT_STATE + 5:
		case CPUINFO_INT_INPUT_STATE + 6:
		case CPUINFO_INT_INPUT_STATE + 7:
		case CPUINFO_INT_INPUT_STATE + INPUT_LINE_NMI:
			set_irq_line(m68k, state - CPUINFO_INT_INPUT_STATE, info->i);
			break;
	}
}

static CPU_EXPORT_STRING( m68k )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	if (entry.index() == STATE_GENFLAGS)
	{
			UINT16 sr = m68ki_get_sr(m68k);
			string.printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
				sr & 0x8000 ? 'T':'.',
				sr & 0x4000 ? 't':'.',
				sr & 0x2000 ? 'S':'.',
				sr & 0x1000 ? 'M':'.',
				sr & 0x0800 ? '?':'.',
				sr & 0x0400 ? 'I':'.',
				sr & 0x0200 ? 'I':'.',
				sr & 0x0100 ? 'I':'.',
				sr & 0x0080 ? '?':'.',
				sr & 0x0040 ? '?':'.',
				sr & 0x0020 ? '?':'.',
				sr & 0x0010 ? 'X':'.',
				sr & 0x0008 ? 'N':'.',
				sr & 0x0004 ? 'Z':'.',
				sr & 0x0002 ? 'V':'.',
				sr & 0x0001 ? 'C':'.');
	}
}

static CPU_GET_INFO( m68k )
{
	m68ki_cpu_core *m68k = (device != NULL && device->token() != NULL) ? get_safe_token(device) : NULL;

	switch (state)
	{
		/* --- the following bits of info are returned as 64-bit signed integers --- */
		case CPUINFO_INT_CONTEXT_SIZE: info->i = sizeof(m68ki_cpu_core); break;
		case CPUINFO_INT_INPUT_LINES: info->i = 8; break;
		case CPUINFO_INT_DEFAULT_IRQ_VECTOR: info->i = -1; break;
		case DEVINFO_INT_ENDIANNESS: info->i = ENDIANNESS_BIG; break;

		case CPUINFO_INT_CLOCK_MULTIPLIER: info->i = 1; break;
		case CPUINFO_INT_CLOCK_DIVIDER: info->i = 1; break;

		case CPUINFO_INT_MIN_INSTRUCTION_BYTES: info->i = 2; break;
		case CPUINFO_INT_MAX_INSTRUCTION_BYTES: info->i = 10; break;

		case CPUINFO_INT_MIN_CYCLES: info->i = 4; break;
		case CPUINFO_INT_MAX_CYCLES: info->i = 158; break;

		case DEVINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_PROGRAM:	info->i = 16; break;
		case DEVINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_PROGRAM:	info->i = 24; break;
		case DEVINFO_INT_ADDRBUS_SHIFT + ADDRESS_SPACE_PROGRAM:	info->i = 0; break;

		case CPUINFO_INT_INPUT_STATE + 0: info->i = 0; /* there is no level 0 */ break;
		case CPUINFO_INT_INPUT_STATE + 1: info->i = (m68k->virq_state >> 1) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 2: info->i = (m68k->virq_state >> 2) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 3: info->i = (m68k->virq_state >> 3) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 4: info->i = (m68k->virq_state >> 4) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 5: info->i = (m68k->virq_state >> 5) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 6: info->i = (m68k->virq_state >> 6) & 0x01; break;
		case CPUINFO_INT_INPUT_STATE + 7: info->i = (m68k->virq_state >> 7) & 0x01; break;

		/* --- the following bits of info are returned as pointers to functions --- */
		case CPUINFO_FCT_SET_INFO: info->setinfo = CPU_SET_INFO_NAME(m68k); break;
		case CPUINFO_FCT_INIT: /* set per-core */ break;
		case CPUINFO_FCT_RESET: info->reset = CPU_RESET_NAME(m68k); break;
		case CPUINFO_FCT_EXECUTE: info->execute = CPU_EXECUTE_NAME(m68k); break;
		case CPUINFO_FCT_DISASSEMBLE: info->disassemble = CPU_DISASSEMBLE_NAME(m68k); break;
		case CPUINFO_FCT_IMPORT_STATE: info->import_state = CPU_IMPORT_STATE_NAME(m68k); break;
		case CPUINFO_FCT_EXPORT_STATE: info->export_state = CPU_EXPORT_STATE_NAME(m68k); break;
		case CPUINFO_FCT_EXPORT_STRING: info->export_string = CPU_EXPORT_STRING_NAME(m68k); break;
		case CPUINFO_FCT_TRANSLATE: info->translate = CPU_TRANSLATE_NAME(m68k); break;

		/* --- the following bits of info are returned as pointers --- */
		case CPUINFO_PTR_INSTRUCTION_COUNTER: info->icount = &m68k->remaining_cycles; break;

		/* --- the following bits of info are returned as NULL-terminated strings --- */
		case DEVINFO_STR_NAME: /* set per-core */ break;
		case DEVINFO_STR_FAMILY: strcpy(info->s, "Motorola 68K"); break;
		case DEVINFO_STR_VERSION: strcpy(info->s, "4.60"); break;
		case DEVINFO_STR_SOURCE_FILE: strcpy(info->s, __FILE__); break;
		case DEVINFO_STR_CREDITS: strcpy(info->s, "Copyright Karl Stenerud. All rights reserved. (2.1 fixes HJB, FPU+MMU by RB)"); break;
	}
}


/****************************************************************************
 * 8-bit data memory interface
 ****************************************************************************/

static UINT16 m68008_read_immediate_16(address_space *space, offs_t address)
{
	offs_t addr = address;
	return (memory_decrypted_read_byte(space, addr) << 8) | (memory_decrypted_read_byte(space, addr + 1));
}

/* interface for 20/22-bit address bus, 8-bit data bus (68008) */
static const m68k_memory_interface interface_d8 =
{
	0,
	m68008_read_immediate_16,
	memory_read_byte,
	memory_read_word,
	memory_read_dword,
	memory_write_byte,
	memory_write_word,
	memory_write_dword
};

/****************************************************************************
 * 16-bit data memory interface
 ****************************************************************************/

static UINT16 simple_read_immediate_16(address_space *space, offs_t address)
{
	return memory_decrypted_read_word(space, address);
}

/* interface for 24-bit address bus, 16-bit data bus (68000, 68010) */
static const m68k_memory_interface interface_d16 =
{
	0,
	simple_read_immediate_16,
	memory_read_byte,
	memory_read_word,
	memory_read_dword,
	memory_write_byte,
	memory_write_word,
	memory_write_dword
};


/****************************************************************************
 * State definition
 ****************************************************************************/

static void define_state(running_device *device)
{
	m68ki_cpu_core *m68k = get_safe_token(device);
	device_state_interface *state;
	device->interface(state);

	state->state_add(M68K_PC,         "PC",        m68k->pc).mask(0x00ffffff);
	state->state_add(STATE_GENPC,     "GENPC",     m68k->pc).mask(0x00ffffff).noshow();
	state->state_add(STATE_GENPCBASE, "GENPCBASE", m68k->ppc).mask(0x00ffffff).noshow();
	state->state_add(M68K_SP,         "SP",        m68k->dar[15]);
	state->state_add(STATE_GENSP,     "GENSP",     m68k->dar[15]).noshow();
	state->state_add(STATE_GENFLAGS,  "GENFLAGS",  m68k->iotemp).noshow().callimport().callexport().formatstr("%16s");
	state->state_add(M68K_ISP,        "ISP",       m68k->iotemp).callimport().callexport();
	state->state_add(M68K_USP,        "USP",       m68k->iotemp).callimport().callexport();
	state->state_add(M68K_ISP,        "ISP",       m68k->iotemp).callimport().callexport();

	astring tempstr;
	int regnum;
	for (regnum = 0; regnum < 8; regnum++)
		state->state_add(M68K_D0 + regnum, tempstr.format("D%d", regnum), m68k->dar[regnum]);
	for (regnum = 0; regnum < 8; regnum++)
		state->state_add(M68K_A0 + regnum, tempstr.format("A%d", regnum), m68k->dar[8 + regnum]);

	state->state_add(M68K_PREF_ADDR,  "PREF_ADDR", m68k->pref_addr).mask(0x00ffffff);
	state->state_add(M68K_PREF_DATA,  "PREF_DATA", m68k->pref_data);
}


/****************************************************************************
 * 68000 section
 ****************************************************************************/

static CPU_INIT( m68000 )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	CPU_INIT_CALL(m68k);

	m68k->cpu_type         = CPU_TYPE_000;
	m68k->dasm_type        = M68K_CPU_TYPE_68000;
	m68k->memory           = interface_d16;
	m68k->sr_mask          = 0xa71f;		/* T1 -- S  -- -- I2 I1 I0 -- -- -- X  N  Z  V  C  */
	m68k->cyc_instruction  = m68ki_cycles;
	m68k->cyc_exception    = m68ki_exception_cycle_table;
	m68k->cyc_bcc_notake_b = -2;
	m68k->cyc_bcc_notake_w = 2;
	m68k->cyc_dbcc_f_noexp = -2;
	m68k->cyc_dbcc_f_exp   = 2;
	m68k->cyc_scc_r_true   = 2;
	m68k->cyc_movem_w      = 2;
	m68k->cyc_movem_l      = 3;
	m68k->cyc_shift        = 1;
	m68k->cyc_reset        = 132;

	define_state(device);
}

CPU_GET_INFO( m68000 )
{
	switch (state)
	{
		/* --- the following bits of info are returned as pointers to data or functions --- */
		case CPUINFO_FCT_INIT: info->init = CPU_INIT_NAME(m68000); break;

		/* --- the following bits of info are returned as NULL-terminated strings --- */
		case DEVINFO_STR_NAME: strcpy(info->s, "68000"); break;

		default: CPU_GET_INFO_CALL(m68k); break;
	}
}


/****************************************************************************
 * M68008 section
 ****************************************************************************/

static CPU_INIT( m68008 )
{
	m68ki_cpu_core *m68k = get_safe_token(device);

	CPU_INIT_CALL(m68k);

	m68k->cpu_type         = CPU_TYPE_008;
	m68k->dasm_type        = M68K_CPU_TYPE_68008;
	m68k->memory           = interface_d8;
	m68k->sr_mask          = 0xa71f;		/* T1 -- S  -- -- I2 I1 I0 -- -- -- X  N  Z  V  C  */
	m68k->cyc_instruction  = m68ki_cycles;
	m68k->cyc_exception    = m68ki_exception_cycle_table;
	m68k->cyc_bcc_notake_b = -2;
	m68k->cyc_bcc_notake_w = 2;
	m68k->cyc_dbcc_f_noexp = -2;
	m68k->cyc_dbcc_f_exp   = 2;
	m68k->cyc_scc_r_true   = 2;
	m68k->cyc_movem_w      = 2;
	m68k->cyc_movem_l      = 3;
	m68k->cyc_shift        = 1;
	m68k->cyc_reset        = 132;

	define_state(device);
}

CPU_GET_INFO( m68008 )
{
	switch (state)
	{
		/* --- the following bits of info are returned as 64-bit signed integers --- */
		case DEVINFO_INT_DATABUS_WIDTH + ADDRESS_SPACE_PROGRAM: info->i = 8; break;
		case DEVINFO_INT_ADDRBUS_WIDTH + ADDRESS_SPACE_PROGRAM: info->i = 22; break;

		/* --- the following bits of info are returned as pointers to data or functions --- */
		case CPUINFO_FCT_INIT: info->init = CPU_INIT_NAME(m68008); break;

		/* --- the following bits of info are returned as NULL-terminated strings --- */
		case DEVINFO_STR_NAME: strcpy(info->s, "68008"); break;

		default: CPU_GET_INFO_CALL(m68k); break;
	}
}


DEFINE_LEGACY_CPU_DEVICE(M68000, m68000);
DEFINE_LEGACY_CPU_DEVICE(M68008, m68008);
