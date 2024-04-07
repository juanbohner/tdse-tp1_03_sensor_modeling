/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_b.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "board.h"
#include "app.h"
#include "task_b_fsm.h"

/********************** macros and definitions *******************************/
#define G_TASK_B_CNT_INI	0u

#define TASK_B_DEL_INI		0u
#define TASK_B_DEL_MAX		50u

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
const char *p_task_b 		= "Task B (BUTTON) (Sensor Modeling)";

/********************** external data declaration *****************************/
uint32_t g_task_b_cnt;

s_task_b_t g_state_task_b;

e_task_b_t g_event_task_b;
bool g_b_event_task_b;


const char *const names_e_task_b[] = {
        "ev_BUTTON_PRESSED",
        "ev_BUTTON_HOVERED"
    };

const char *const names_s_task_b[] = {
        "BUTTON_OPEN_st",
        "BUTTON_BOUNCING_st",
        "BUTTON_VERIFICATION_st",
        "BUTTON_CLOSED_st"
    };


/********************** external functions definition ************************/
void task_b_init(void *parameters)
{

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_b_init), p_task_b);

	g_task_b_cnt = G_TASK_B_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_b_cnt), (int)g_task_b_cnt);

	g_state_task_b = BUTTON_OPEN_st;
	g_event_task_b = ev_BUTTON_HOVERED;
	g_b_event_task_b = false;

	/***
	 *
	 * MI CAMBIO - START
	 * ***/
	/* Print out: Task execution FSM */
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(g_state_task_b), names_s_task_b[(int)g_state_task_b]);
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(g_event_task_b), names_e_task_b[(int)g_event_task_b]);
	/***
	 *
	 * MI CAMBIO - END
	 * ***/
}


/***
 *
 * MI CAMBIO - START
 * ***/
void task_b_update(void *parameters)
{   
    static uint32_t then = TASK_B_DEL_INI;
	static uint32_t now = TASK_B_DEL_INI;

	/* Update Task A Counter */
	g_task_b_cnt++;


    now = HAL_GetTick();
	
	if (true == g_b_event_task_b)
	{
		g_b_event_task_b = false;

		/* Print out: Application Update */
		LOGGER_LOG("  %s is is running - %s\r\n", GET_NAME(task_b_update), p_task_b);

		/* Print out: Task execution counter */
		LOGGER_LOG("   %s = %d\r\n", GET_NAME(g_task_b_cnt), (int)g_task_b_cnt);

		switch (g_state_task_b){
			case BUTTON_OPEN_st:
				if (ev_BUTTON_PRESSED == g_event_task_b)
					g_state_task_b = BUTTON_BOUNCING_st;

				then = now;
				break;

			case BUTTON_BOUNCING_st:
				//Si pasan 50 ms tras que se considera tocado el botón 
                if ((now - then) >= TASK_B_DEL_MAX)
                {
                    /* Only if the current tick is TASK_B_DEL_MAX mS after the last */
                    /* Reset then = now */
                    then = now;
                    LOGGER_LOG("SI Entro\r\n");
                    g_state_task_b = BUTTON_VERIFICATION_st;
                }
                else{
                	LOGGER_LOG("NO Entro\r\n");
                }

				break;

			case BUTTON_VERIFICATION_st:
				if (ev_BUTTON_HOVERED == g_event_task_b)
					g_state_task_b = BUTTON_OPEN_st;

                if (ev_BUTTON_PRESSED == g_event_task_b)
					g_state_task_b = BUTTON_CLOSED_st;

				break;

			case BUTTON_CLOSED_st:
				//Hacer algo porque se toca el botón 
                //Prender el LED

                if (ev_BUTTON_HOVERED == g_event_task_b)
                    g_state_task_b = BUTTON_OPEN_st;
				
                break;

            default:
                LOGGER_LOG("ERROR - default case - Task b (Boton) \r\n");
                break;

		}
		/* Print out: Task execution FSM */
		LOGGER_LOG("   %s = %s\r\n", GET_NAME(g_state_task_b), names_s_task_b[(int)g_state_task_b]);
		LOGGER_LOG("   %s = %s\r\n", GET_NAME(g_event_task_b), names_e_task_b[(int)g_event_task_b]);
	}
}

	/***
	 *
	 * MI CAMBIO - END
	 * ***/

/********************** end of file ******************************************/
