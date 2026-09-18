/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2018 Google LLC
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#ifndef _OS_HW_H
#define _OS_HW_H

#include <stdbool.h>

#include "FreeRTOS.h"
#include "task.h"

#if defined(xPortIsInsideInterrupt) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_8M_MAIN__)
static inline bool pos_hw_in_isr(void)
{
    return xPortIsInsideInterrupt() == pdTRUE;
}
#elif defined(CHIP_DEVICE_LAYER_TARGET_NRF5)
#include <nrf52840.h>
static inline bool pos_hw_in_isr(void)
{
    return (SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) != 0;
}
#else
static inline bool pos_hw_in_isr(void)
{
    return false;
}
#endif

#endif /* _OS_HW_H */
