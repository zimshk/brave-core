/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as noScriptTypes from '../../constants/noScriptTypes'
import * as webNavigationTypes from '../../constants/webNavigationTypes'
import { State } from '../../types/state/shieldsPannelState'
import { Actions } from '../../types/actions'
import { getActiveTabId, getActiveTabData } from '../../state/shieldsPanelState'
import { setAllowJavaScript, setAllowScriptOriginsOnce } from '../../background/api/noScriptAPI'
import { requestShieldPanelData, toggleShieldsValue } from '../../background/api/shieldsAPI'
import { reloadTab } from '../../background/api/tabsAPI'
import {
  setNoScriptInfo,
  filterNoScriptInfoByBlockedState,
  getHostname,
  getNoScriptInfo
} from '../../helpers/noScriptUtils'

export default function noScriptReducer (
  state: State = { tabs: {}, windows: {}, currentWindowId: -1 },
  action: Actions
) {

  switch (action.type) {
    case webNavigationTypes.ON_COMMITTED: {
      // if (action.isMainFrame) {
      //   state = resetNoScriptInfo(state, action.tabId, new window.URL(action.url).origin)
      // }
      break
    }

    case noScriptTypes.JAVASCRIPT_TOGGLED: {
      const tabData = getActiveTabData(state)
      if (!tabData) {
        console.error('Active tab not found')
        break
      }
      setAllowJavaScript(tabData.origin, toggleShieldsValue(tabData.javascript))
        .then(() => {
          requestShieldPanelData(getActiveTabId(state))
          reloadTab(tabData.id, true).catch(() => {
            console.error('Tab reload was not successful')
          })
        })
        .catch(() => {
          console.error('Could not set JavaScript setting')
        })
      break
    }

    case noScriptTypes.ALLOW_SCRIPT_ORIGINS_ONCE: {
      const tabData = getActiveTabData(state)
      if (!tabData) {
        console.error('Active tab not found')
        break
      }
      setAllowScriptOriginsOnce(action.origins, tabData.id)
        .then(() => {
          requestShieldPanelData(getActiveTabId(state))
          reloadTab(tabData.id, true).catch(() => {
            console.error('Tab reload was not successful')
          })
        })
        .catch(() => {
          console.error('Could not set allow script origins once')
        })
      break
    }

    // NoScriptInfo is the name we call for the list of scripts that are either
    // blocked or allowed by the user. Each script have three properties:
    // ....................................................................................
    // `actuallyBlocked`:
    // ....................................................................................
    // When set to `true` it blocks the script immediatelly. This is the initial state
    // when the user toggle scripts blocked in the main panel screen and also the initial state
    // for when users toggle `block/allow` or `block all/allow all`
    // ....................................................................................
    // `willBlock`:
    // ....................................................................................
    // When set to `true` it moves the script to its respective list. This is the final state
    // when the user choose to close Shields either by clicking `cancel`, moving back to the
    // main screen, or closing Shields browser action. This state is triggered only after those actions
    // and its state inherit the state of `actuallyBlocked`.
    // ....................................................................................
    // `userInteracted`:
    // ....................................................................................
    // This property is for display only. With this we can tell whether or not the user have
    // interacted with the script which can change the button state to allow/block (no user interaction)
    // or blocked once/allowed once (user has interacted).

    // Used for standalone scripts i.e. not grouped by other scripts in the same hostname
    case noScriptTypes.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE: {
      const noScriptInfo = getNoScriptInfo(state)
      const tabId: number = getActiveTabId(state)
      const scriptBlockedState = {
        userInteracted: true,
        actuallyBlocked: !noScriptInfo[action.url].actuallyBlocked
      }
      state = setNoScriptInfo(state, tabId, action.url, scriptBlockedState)
      break
    }

    case noScriptTypes.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE: {
      const noScriptInfo = getNoScriptInfo(state)
      const tabId: number = getActiveTabId(state)
      const groupedScripts = Object.entries(noScriptInfo)
      const maybeBlockGroup = action.maybeBlock
      for (const [url] of filterNoScriptInfoByBlockedState(groupedScripts, maybeBlockGroup)) {
        const groupedScriptsBlockedState = {
          userInteracted: true,
          actuallyBlocked: !noScriptInfo[url].actuallyBlocked
        }

        if (action.hostname === getHostname(url)) {
          state = setNoScriptInfo(state, tabId, url, groupedScriptsBlockedState)
        }
      }
      break
    }

    case noScriptTypes.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE: {
      const noScriptInfo = getNoScriptInfo(state)
      const tabId: number = getActiveTabId(state)
      const allBlockedScripts = Object.entries(noScriptInfo)
      const maybeBlockAll = action.maybeBlock
      for (const [url] of filterNoScriptInfoByBlockedState(allBlockedScripts, maybeBlockAll, true)) {
        const groupedScriptsBlockedState = {
          userInteracted: true,
          actuallyBlocked: !noScriptInfo[url].actuallyBlocked
        }

        state = setNoScriptInfo(state, tabId, url, groupedScriptsBlockedState)
      }
      break
    }

    case noScriptTypes.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE: {
      const noScriptInfo = getNoScriptInfo(state)
      const tabId: number = getActiveTabId(state)
      const allScripts = Object.entries(noScriptInfo)
      for (const [url] of allScripts) {
        // `willBlock` set the state once user close Shields panel moving blocked/allowed resources
        // to their respective lists. In this case we copy data from actuallyBlocked and apply
        // the sate state to willBlock so users can see their scripts list updated.
        const groupedScriptsBlockedState = {
          userInteracted: false,
          willBlock: noScriptInfo[url].actuallyBlocked
        }
        state = setNoScriptInfo(state, tabId, url, groupedScriptsBlockedState)
      }
      break
    }
  }
  return state
}
