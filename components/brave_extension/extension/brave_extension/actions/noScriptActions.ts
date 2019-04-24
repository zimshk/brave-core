/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// Types
import * as types from '../constants/noScriptTypes'
import * as actions from '../types/actions/noScriptActions'

/**
 * Set a given script resource state to be in the allowed/blocked list
 * @param {string} url - The resource URL
 * @param {boolean} maybeBlock - Whether or not the resource should be blocked
 */
export const setScriptBlockedCurrentState: actions.SetScriptBlockedCurrentState = (
  url: string
) => {
  return {
    type: types.SET_SCRIPT_BLOCKED_ONCE_CURRENT_STATE,
    url
  }
}

/**
 * Set all child resources of a given hostname to be in the allowed/blocked list
 * @param {string} hostname - The blocked resource hostname
 * @param {boolean} maybeBlock - Whether or not the resource should be blocked
 */
export const setGroupedScriptsBlockedCurrentState: actions.SetGroupedScriptsBlockedCurrentState = (
  hostname: string,
  maybeBlock: boolean
) => {
  return {
    type: types.SET_GROUPED_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
    hostname,
    maybeBlock
  }
}

/**
 * Set all resources in a blocked/allowed state to be in the allowed/blocked list
 * @param {boolean} maybeBlock - Whether or not the resource should be blocked
 */
export const setAllScriptsBlockedCurrentState: actions.SetAllScriptsBlockedCurrentState = (
  maybeBlock: boolean
) => {
  return {
    type: types.SET_ALL_SCRIPTS_BLOCKED_ONCE_CURRENT_STATE,
    maybeBlock
  }
}

/**
 * Set the final state to all resources so they could be stored persistently in the blocked/allowed list
 */
export const setFinalScriptsBlockedState: actions.SetFinalScriptsBlockedState = () => {
  return {
    type: types.SET_FINAL_SCRIPTS_BLOCKED_ONCE_STATE
  }
}

/**
 * Set all scripts in an array to be allowed while the session is open
 * @param {Array<string>} origins - an array of origins to be set as allowed for one time
 */
export const allowScriptOriginsOnce: actions.AllowScriptOriginsOnce = (origins: Array<string>) => {
  return {
    type: types.ALLOW_SCRIPT_ORIGINS_ONCE,
    origins
  }
}

/**
 * Toggle all scripts blocked to be either allowed or blocked
 * @param {'allow' | 'block'} setting - whether or not the scripts are allowed or blocked
 */
export const blockJavaScript: actions.BlockJavaScript = (setting: 'allow' | 'block') => {
  return {
    type: types.JAVASCRIPT_TOGGLED,
    setting
  }
}
