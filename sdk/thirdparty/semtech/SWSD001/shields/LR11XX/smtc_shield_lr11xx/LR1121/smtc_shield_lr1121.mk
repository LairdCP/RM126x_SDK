# --- The Clear BSD License ---
# Copyright Semtech Corporation 2022. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted (subject to the limitations in the disclaimer
# below) provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Semtech corporation nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY
# THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
# CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
# NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
# PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SEMTECH CORPORATION BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

define SMTC_SHIELD_LR1121_SUPPORTED_BODY
Shield $(SMTC_SHIELD_LR11XX) is not supported.

The LR1121 supported shields are: LR1121MB1DIS / LR1121MB1GIS
endef

ifeq ($(SMTC_SHIELD_LR11XX), LR1121MB1DIS)
C_SOURCES += \
$(SMTC_SHIELD_LR11XX_DIR)/smtc_shield_lr11xx/LR1121/LR1121MB1DIS/smtc_shield_lr1121mb1dis.c

else ifeq ($(SMTC_SHIELD_LR11XX), LR1121MB1GIS)
C_SOURCES += \
$(SMTC_SHIELD_LR11XX_DIR)/smtc_shield_lr11xx/LR1121/LR1121MB1GIS/smtc_shield_lr1121mb1gis.c

else
$(error $(SMTC_SHIELD_LR1121_SUPPORTED_BODY))
endif
