/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as types from '../../../brave_extension/extension/brave_extension/constants/noScriptTypes'
import * as actions from '../../../brave_extension/extension/brave_extension/actions/noScriptActions'
import { BlockOptions } from '../../../brave_extension/extension/brave_extension/types/other/blockTypes'

describe('shieldsPanelActions', () => {
  it('blockJavaScript action', () => {
    const setting: BlockOptions = 'allow'
    expect(actions.blockJavaScript(setting)).toEqual({
      type: types.JAVASCRIPT_TOGGLED,
      setting
    })
  })

  it('allowScriptOriginsOnce action', () => {
    const origins = ['https://a.com', 'https://b.com']
    expect(actions.allowScriptOriginsOnce(origins)).toEqual({
      type: types.ALLOW_SCRIPT_ORIGINS_ONCE,
      origins
    })
  })

  it('setScriptBlockedCurrentState', () => {
    const url = 'https://awesome-anthony-tseng.website'
    expect(actions.setScriptBlockedCurrentState(url)).toEqual({
      type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
      url
    })
  })

  it('setGroupedScriptsBlockedCurrentState', () => {
    const hostname = 'https://clifton.io'
    const maybeBlock = false
    expect(actions.setGroupedScriptsBlockedCurrentState(hostname, maybeBlock)).toEqual({
      type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
      hostname,
      maybeBlock
    })
  })

  it('setAllScriptsBlockedCurrentState', () => {
    const maybeBlock = true
    expect(actions.setAllScriptsBlockedCurrentState(maybeBlock)).toEqual({
      type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
      maybeBlock
    })
  })

  it('setFinalScriptsBlockedState', () => {
    expect(actions.setFinalScriptsBlockedState()).toEqual({
      type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
    })
  })
})
