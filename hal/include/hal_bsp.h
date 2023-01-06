/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*-
 * Copyright (C) 2013. VeriSilicon Holdings Co., Ltd. All rights reserved.
 * Author: zhihui.ye@verisilicon.com, IoT Connectivity Platform
 * Date: 2016-08-26
 * Brief: This file is modified from NIMBLE project, to fit VeriSilicon's
 * innovative BLE HW product
 */

#ifndef __HAL_BSP_H_
#define __HAL_BSP_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initializes BSP; registers flash_map with the system.
 *
 */
void bsp_init(void);

#ifdef __cplusplus
}
#endif

#endif
