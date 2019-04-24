/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import { NoScriptInfo } from '../types/other/noScriptInfo'
import { State, Tabs, Tab } from '../types/state/shieldsPannelState'
import { getActiveTabData } from '../state/shieldsPanelState'
import { getOrigin } from './urlUtils'

/**
 * Filter resources by origin to be used for generating NoScriptInfo.
 * @param {NoScriptInfo} noScriptInfo - The NoScriptInfo state
 * @param {string} url - The URL to compare origins against each other
 */
export const filterResourceByOrigin = (noScriptInfo: NoScriptInfo, url: string) => {
  return Object.entries(noScriptInfo).filter((script) => {
    return getOrigin(url) === getOrigin(script[0])
  })
}

/**
 * Generate data structure for the NoScript object.
 * This is useful to group scripts by origin and is used for presentational
 * purposes only, as data is stored same way as it comes from the back-end.
 * @param {NoScriptInfo} noScriptInfo - The NoScriptInfo state
 */
export const generateNoScriptInfoDataStructure = (noScriptInfo: NoScriptInfo) => {
  let newData = []
  for (const [url] of Object.entries(noScriptInfo)) {
    const entry = newData.some((item) => item[0] === getOrigin(url))
    if (!entry) {
      newData.push([ getOrigin(url), filterResourceByOrigin(noScriptInfo, url) ])
    }
  }
  return newData
}

/**
 * Filter NoScriptInfo by `willBlock` state
 * @param {Array<any>} modifiedNoScriptInfo - The NoScriptInfo state
 * @param {boolean} maybeBlock - Whether or not the resource should be blocked
 * @param {boolean} filterByDifference - Whether or not `willBlock` should be filtered by difference
 */
export const filterNoScriptInfoByBlockedState = (
  modifiedNoScriptInfo: Array<any>,
  maybeBlock: boolean,
  filterByDifference?: boolean
) => {
  if (filterByDifference) {
    return modifiedNoScriptInfo.filter(script => script[1].willBlock !== maybeBlock)
  }
  return modifiedNoScriptInfo.filter(script => script[1].willBlock === maybeBlock)
}

/**
 * Get NoScriptInfo initial state
 * @param {State} state - The initial NoScriptState
 */
export const getNoScriptInfo = (state: State) => {
  const tabData: Tab = getActiveTabData(state)
  return tabData.noScriptInfo
}

/**
 * Set NoScriptInfo modified state
 * @param {State} state - The Application state
 * @param {string} url - The current script URL
 * @param {object} modifiedInfo - The current script URL object data to be modified
 */
export const setNoScriptInfo = (state: State, tabId: number, url: string, modifiedInfo: {}) => {
  const tabs: Tabs = {
    ...state.tabs,
    [tabId]: {
      ...state.tabs[tabId],
      noScriptInfo: {
        ...state.tabs[tabId].noScriptInfo,
        [url]: {
          ...state.tabs[tabId].noScriptInfo[url],
          ...modifiedInfo
        }
      }
    }
  }

  return { ...state, tabs }
}

/**
 * Check if all scripts in NoScriptInfo are either allowed or blocked by the user
 * @param {Array<any>} modifiedNoScriptInfo - The modifiedNoScriptInfo state
 * @param {boolean} isBlocked - Whether or not all scripts are blocked
 */
export const checkEveryItemIsBlockedOrAllowed = (
  modifiedNoScriptInfo: Array<any>,
  isBlocked: boolean
) => {
  return modifiedNoScriptInfo
    .filter(script => script[1].willBlock === isBlocked)
    .every(script => script[1].userInteracted)
}

/**
 * Get script "block all"/"allow all" text
 * Scripts are divided between blocked/allowed and we have an option to block/allow all.
 * If all scripts in a list are set to blocked/allowed, state should change
 * to "allowed once" or "blocked once"
 * @param {NoScriptInfo} noScriptInfo - The NoScriptInfo state
 * @param {boolean} isBlocked - Whether or not all scripts are blocked
 */
export const getBlockAllText = (
  noScriptInfo: NoScriptInfo,
  isBlocked: boolean
) => {
  const everyItemIsWasInteracted = Object.entries(noScriptInfo)
    .every(data => data[1].willBlock === isBlocked && data[1].userInteracted)

  if (isBlocked) {
    if (everyItemIsWasInteracted) {
      return 'Allowed once'
    }
    return 'Allow all'
  } else {
    if (everyItemIsWasInteracted) {
      return 'Blocked once'
    }
    return 'Block all'
  }
}

/**
 * Get script "block" text
 * Scripts can be set as allow/block when there is no interaction
 * and allowed once/blocked once when interaction have happened
 * @param {boolean} haveUserInteracted - Whether or not user have interacted with the script
 * @param {boolean} isBlocked - Whether or not the current script is blocked
 */
export const getBlockScriptText = (haveUserInteracted: boolean, isBlocked: boolean) => {
  if (!haveUserInteracted) {
    return isBlocked ? 'Allow' : 'Block'
  }
  return isBlocked ? 'Allowed once' : 'Blocked once'
}
