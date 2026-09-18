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

#include <poski/osal/osal.h>
#include <poski/osal/os_sched.h>

#include <assert.h>

static void task_wrapper(void * arg)
{
    struct pos_task * task = (struct pos_task *) arg;
    if (task->func)
    {
        task->func(task->arg);
    }
    vTaskDelete(NULL);
}

pos_error_t pos_task_init(struct pos_task * task, const char * name, pos_task_func_t func, void * arg, uint8_t prio,
                          uint16_t stack_size)
{
    BaseType_t err;
    if ((task == NULL) || (func == NULL))
    {
        return POS_INVALID_PARAM;
    }

    task->func = func;
    task->arg  = arg;

    err = xTaskCreate(task_wrapper, name, stack_size, task, prio, &task->handle);

    return (err == pdPASS) ? POS_OK : POS_ENOMEM;
}

pos_error_t pos_task_remove(struct pos_task * t)
{
    TaskHandle_t h = NULL;

    if (t == NULL)
    {
        return POS_INVALID_PARAM;
    }

    taskENTER_CRITICAL();
    h = t->handle;
    if (h != NULL)
    {
        t->handle = NULL;
    }
    taskEXIT_CRITICAL();

    if (h == NULL)
    {
        return POS_INVALID_PARAM;
    }

    vTaskDelete(h);
    return POS_OK;
}

bool pos_sched_started(void)
{
    return pos_os_started();
}
