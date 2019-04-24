/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Types
import * as types from '../../../../brave_extension/extension/brave_extension/constants/noScriptTypes'
// import * as webNavigationTypes from '../../../../brave_extension/extension/brave_extension/constants/webNavigationTypes'
import { State } from '../../../../brave_extension/extension/brave_extension/types/state/shieldsPannelState'
import * as deepFreeze from 'deep-freeze-node'
import noScriptReducer from '../../../../brave_extension/extension/brave_extension/background/reducers/noScriptReducer'
import { getHostname } from '../../../../brave_extension/extension/brave_extension/helpers/urlUtils'
import * as noScriptAPI from '../../../../brave_extension/extension/brave_extension/background/api/noScriptAPI'
import * as tabsAPI from '../../../../brave_extension/extension/brave_extension/background/api/tabsAPI'
// import * as shieldsPanelState from '../../../../brave_extension/extension/brave_extension/state/shieldsPanelState'

const url: string = 'https://brave.com'
const scriptUrl1: string = 'http://super-malicious-tracker.com/malicious.js'
const scriptUrl2: string = 'http://super-malicious-tracker.com/another-malicious-script.js'
const tabId: number = 2
const state: State = deepFreeze({
  tabs: {
    [tabId]: {
      origin: url,
      id: tabId,
      noScriptInfo: {
        [scriptUrl1]: { actuallyBlocked: true, willBlock: true, userInteracted: false },
        [scriptUrl2]: { actuallyBlocked: false, willBlock: false, userInteracted: false }
      }
    }
  },
  windows: { 1: tabId },
  currentWindowId: 1
})

describe('braveNoScriptReducer', () => {
  // describe('ON_COMMITTED', () => {
  //   let resetNoScriptInfoSpy: jest.SpyInstance
  //   beforeEach(() => {
  //     resetNoScriptInfoSpy = jest.spyOn(shieldsPanelState, 'resetNoScriptInfo')
  //   })
  //   afterEach(() => {
  //     resetNoScriptInfoSpy.mockRestore()
  //   })
  //   it.skip('calls resetNoScriptInfo when isMainFrame is true', () => {
  //     noScriptReducer(state, {
  //       type: webNavigationTypes.ON_COMMITTED,
  //       tabId,
  //       url,
  //       isMainFrame: true
  //     })
  //     expect(resetNoScriptInfoSpy).toBeCalledTimes(1)
  //     expect(resetNoScriptInfoSpy.mock.calls[0][1]).toBe(tabId)
  //     expect(resetNoScriptInfoSpy.mock.calls[0][2]).toBe('https://www.brave.com')
  //   })
  //   it.skip('does not call resetNoScriptInfo when isMainFrame is false', () => {
  //     noScriptReducer(state, {
  //       type: webNavigationTypes.ON_COMMITTED,
  //       tabId,
  //       url,
  //       isMainFrame: false
  //     })
  //     expect(resetNoScriptInfoSpy).not.toBeCalled()
  //   })
  // })

  describe('ALLOW_SCRIPT_ORIGINS_ONCE', () => {
    let reloadTabSpy: jest.SpyInstance
    let setAllowScriptOriginsOnceSpy: jest.SpyInstance
    beforeEach(() => {
      reloadTabSpy = jest.spyOn(tabsAPI, 'reloadTab')
      setAllowScriptOriginsOnceSpy = jest.spyOn(noScriptAPI, 'setAllowScriptOriginsOnce')
    })
    afterEach(() => {
      reloadTabSpy.mockRestore()
      setAllowScriptOriginsOnceSpy.mockRestore()
    })
    it('should call setAllowScriptOriginsOnce', () => {
      const origins = ['https://a.com/', 'https://b.com/']
      const tabId = 2
      expect(
        noScriptReducer(state, {
          type: types.ALLOW_SCRIPT_ORIGINS_ONCE,
          origins
        })).toEqual(state)
      expect(setAllowScriptOriginsOnceSpy).toBeCalledWith(origins, tabId)
    })
  })

  describe('JAVASCRIPT_TOGGLED', () => {
    let reloadTabSpy: jest.SpyInstance
    let setAllowJavaScriptSpy: jest.SpyInstance
    beforeEach(() => {
      reloadTabSpy = jest.spyOn(tabsAPI, 'reloadTab')
      setAllowJavaScriptSpy = jest.spyOn(noScriptAPI, 'setAllowJavaScript')
    })
    afterEach(() => {
      reloadTabSpy.mockRestore()
      setAllowJavaScriptSpy.mockRestore()
    })
    it('should call setAllowJavaScript', () => {
      expect(
        noScriptReducer(state, {
          type: types.JAVASCRIPT_TOGGLED,
          setting: 'allow'
        })).toEqual(state)
      expect(setAllowJavaScriptSpy).toBeCalledWith(url, 'allow')
    })
  })

  describe('SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE', () => {
    it('set userInteracted to true', () => {
      const url = scriptUrl1
      const modifiedState = noScriptReducer(state, {
        type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
        url
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[url].userInteracted
      expect(assertion).toBe(true)
    })

    it('set actuallyBlocked to false if its true', () => {
      const url = scriptUrl1
      const modifiedState = noScriptReducer(state, {
        type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
        url
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[url].actuallyBlocked
      expect(assertion).toBe(false)
    })

    it('set actuallyBlocked to true if its false', () => {
      const url = scriptUrl2
      const modifiedState = noScriptReducer(state, {
        type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
        url
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[url].actuallyBlocked
      expect(assertion).toBe(true)
    })
  })

  describe('SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE', () => {
    it('set userInteracted to true if hostname match', () => {
      const url = scriptUrl1
      const hostname = getHostname(url)
      const modifiedState = noScriptReducer(state, {
        type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        hostname,
        maybeBlock: true
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[scriptUrl1].userInteracted
      expect(assertion).toBe(true)
    })

    it('set actuallyBlocked to false if its true', () => {
      const url = scriptUrl1
      const hostname = getHostname(url)
      const modifiedState = noScriptReducer(state, {
        type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        hostname,
        maybeBlock: true
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[scriptUrl1].actuallyBlocked
      expect(assertion).toBe(false)
    })

    it('set actuallyBlocked to true if its false', () => {
      const url = scriptUrl2
      const hostname = getHostname(url)
      const modifiedState = noScriptReducer(state, {
        type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        hostname,
        maybeBlock: false
      })
      const assertion = modifiedState.tabs[tabId].noScriptInfo[scriptUrl1].actuallyBlocked
      expect(assertion).toBe(true)
    })

    it('does not modify state if hostname does not match', () => {
      const url = 'https://malicious-scripts-strike-back.com'
      const hostname = getHostname(url)
      const modifiedState = noScriptReducer(state, {
        type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        hostname,
        maybeBlock: true
      })
      const assertion = modifiedState
      expect(assertion).toBe(state)
    })
  })

  describe('SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE', () => {
    it('set all userInteracted to true', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], userInteracted: false },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], userInteracted: false }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        maybeBlock: false
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].userInteracted)
      expect(assertion).toBe(true)
    })

    it('set all actuallyBlocked to false if they are true', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: true },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: true }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        maybeBlock: false
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].actuallyBlocked)
      expect(assertion).toBe(false)
    })

    it('set all actuallyBlocked to true if they are false', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: false },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: false }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
        maybeBlock: false
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].actuallyBlocked)
      expect(assertion).toBe(true)
    })
  })

  describe('SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE', () => {
    it('set all userInteracted to false', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], userInteracted: true },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], userInteracted: true }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].userInteracted)
      expect(assertion).toBe(false)
    })

    it('set all willBlock properties to be true if actuallyBlocked is true', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: true },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: true }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].willBlock)
      expect(assertion).toBe(true)
    })

    it('set all willBlock properties to be false if actuallyBlocked is false', () => {
      const newState: State = {
        ...state,
        tabs: {
          [tabId]: {
            ...state.tabs[tabId],
            noScriptInfo: {
              [scriptUrl1]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: false },
              [scriptUrl2]: { ...state.tabs[tabId].noScriptInfo[scriptUrl1], actuallyBlocked: false }
            }
          }
        }
      }
      const modifiedState = noScriptReducer(newState, {
        type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
      })
      const assertion = Object.entries(modifiedState.tabs[tabId].noScriptInfo)
        .every(script => script[1].willBlock)
      expect(assertion).toBe(false)
    })
  })
})
