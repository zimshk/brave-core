/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as types from '../constants/noScriptTypes'

interface SetScriptBlockedCurrentStateReturn {
  type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
  url: string
}

export interface SetScriptBlockedCurrentState {
  (url: string, maybeBlock: boolean): SetScriptBlockedCurrentStateReturn
}

interface SetGroupedScriptsBlockedCurrentStateReturn {
  type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
  hostname: string,
  maybeBlock: boolean
}

export interface SetGroupedScriptsBlockedCurrentState {
  (hostname: string, maybeBlock: boolean): SetGroupedScriptsBlockedCurrentStateReturn
}

interface SetAllScriptsBlockedCurrentStateReturn {
  type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
  maybeBlock: boolean
}

export interface SetAllScriptsBlockedCurrentState {
  (maybeBlock: boolean): SetAllScriptsBlockedCurrentStateReturn
}

interface SetFinalScriptsBlockedStateReturn {
  type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
}

export interface SetFinalScriptsBlockedState {
  (): SetFinalScriptsBlockedStateReturn
}

export type noScriptActions =
  SetScriptBlockedCurrentStateReturn |
  SetGroupedScriptsBlockedCurrentStateReturn |
  SetAllScriptsBlockedCurrentStateReturn |
  SetFinalScriptsBlockedStateReturn
