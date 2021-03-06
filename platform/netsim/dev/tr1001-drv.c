/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: tr1001-drv.c,v 1.2 2006/10/10 15:58:31 adamdunkels Exp $
 */

#include "contiki-esb.h"
#include "net/hc.h"

#include <string.h>
#include <stdio.h>

PROCESS(tr1001_drv_process, "TR1001 driver");

static unsigned char slip_dump;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(tr1001_drv_process, ev, data)
{
  PROCESS_BEGIN();

  tr1001_init();
  
  while(1) {
    
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_POLL);
    
    uip_len = tr1001_poll();
    
    if(uip_len > 0) {
      uip_len = hc_inflate(&uip_buf[UIP_LLH_LEN], uip_len);
      tcpip_input();
    }
  }
  
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
void
tr1001_drv_request_poll(void)
{
  process_poll(&tr1001_drv_process);
}
/*---------------------------------------------------------------------------*/
u8_t
tr1001_drv_send(void)
{
  uip_len = hc_compress(&uip_buf[UIP_LLH_LEN], uip_len);
  return tr1001_send(&uip_buf[UIP_LLH_LEN], uip_len);
}
/*---------------------------------------------------------------------------*/
void
tr1001_drv_set_slip_dump(int dump)
{
  slip_dump = dump;
}
/*---------------------------------------------------------------------------*/
