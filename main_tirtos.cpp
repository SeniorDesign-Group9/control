/*
 * Copyright (c) 2016-2020, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,

 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <pthread.h>
#include <cstdint>
#include <cstdio>
#include <ti/drivers/Board.h>
#include <ti/drivers/net/wifi/device.h>
#include <ti/sysbios/BIOS.h>

//
extern void *mainThread(void *arg0);

// Stack size in bytes
#define MAINTHREAD_STACKSIZE    8192
#define SLTASK_STACKSIZE        4096

// Main
int main(void) {
    // mainThread
    pthread_t           mainSpawnThread = (pthread_t)NULL;
    pthread_attr_t      mainSpawnAttrs;
    struct sched_param  mainSpawnParam;
    uint32_t            mainSpawnRet = 0;

    // slTask thread
    pthread_t           slSpawnThread = (pthread_t)NULL;
    pthread_attr_t      slSpawnAttrs;
    struct sched_param  slSpawnParam;
    uint32_t            slSpawnRet = 0;

    Board_init();

    // Create mainThread
    pthread_attr_init(&mainSpawnAttrs);

    mainSpawnParam.sched_priority = 1;
    mainSpawnRet = pthread_attr_setschedparam(&mainSpawnAttrs, &mainSpawnParam);
    mainSpawnRet |= pthread_attr_setdetachstate(&mainSpawnAttrs, PTHREAD_CREATE_DETACHED);
    mainSpawnRet |= pthread_attr_setstacksize(&mainSpawnAttrs, MAINTHREAD_STACKSIZE);

    if (mainSpawnRet != 0) {
        // Failed to set attributes
        printf("Attrs mainThread failed.\n");
        while (1) {}
    }

    mainSpawnRet = pthread_create(&mainSpawnThread, &mainSpawnAttrs, mainThread, NULL);
    if (mainSpawnRet != 0) {
        // pthread_create() failed
        printf("Create mainThread failed.\n");
        while (1) {}
    }

    // Create sl_Task
    pthread_attr_init(&slSpawnAttrs);

    slSpawnParam.sched_priority = 9;
    slSpawnRet = pthread_attr_setschedparam(&slSpawnAttrs, &slSpawnParam);
    slSpawnRet |= pthread_attr_setstacksize(&slSpawnAttrs, SLTASK_STACKSIZE);

    if (slSpawnRet != 0) {
        // Failed to set attributes
        printf("Attrs slTask failed.\n");
        while(1) {}
    }

    slSpawnRet = pthread_create(&slSpawnThread, &slSpawnAttrs, sl_Task, NULL);

    if (slSpawnRet != 0) {
        // pthread_create() failed
        printf("Create slTask failed.\n");
        while(1) {}
    }

    // Start RTOS
    BIOS_start();

    return 0;
}
