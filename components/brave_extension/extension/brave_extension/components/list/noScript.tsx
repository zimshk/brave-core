/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

// React API
import * as React from 'react'

// Components
import NoScriptContent from './noScriptsContent'

// Actions
// import * as noScriptActions from '../../actions/noScriptActions'

// Helpers
import {
  filterNoScriptInfoByBlockedState,
  generateNoScriptInfoDataStructure,
  getBlockAllText
} from '../../helpers/noScriptUtils'

import {
  BlockedListHeader,
  BlockedListSummary,
  BlockedListContent,
  BlockedListItemHeader,
  BlockedListDynamic,
  BlockedListFooter,
  ArrowUpIcon,
  LinkAction,
  Favicon,
  SiteInfoText,
  BlockedListSummaryText,
  BlockedListItemHeaderStats,
  BlockedListItemHeaderText,
  ShieldsButton
} from 'brave-ui/features/shields'

import { getLocale } from '../../background/api/localeAPI'
import { NoScriptInfo } from '../../types/other/noScriptInfo'

interface Props {
  favicon: string
  hostname: string
  noScriptInfo: NoScriptInfo
  onClose?: (event?: React.MouseEvent<any>) => void
  actions: any
}

export default class CoreFeature extends React.PureComponent<Props, {}> {
  get noScriptInfo () {
    return this.props.noScriptInfo
  }

  get noScriptActions () {
    return this.props.actions
  }

  get generatedNoScriptData () {
    return generateNoScriptInfoDataStructure(this.noScriptInfo)
  }

  get blockedScriptsLength () {
    return filterNoScriptInfoByBlockedState(Object.entries(this.noScriptInfo), true).length
  }

  get allowedScriptsLength () {
    return filterNoScriptInfoByBlockedState(Object.entries(this.noScriptInfo), false).length
  }

  blockOrAllowAll (blockOrAllow: boolean) {
    this.noScriptActions.setAllScriptsBlockedCurrentState(blockOrAllow)
  }

  setFinalScriptsBlockedState = () => {
    // set willBlock to all actions
    // TODO: this will only apply after clicking "go back" but it shouldn't
    this.noScriptActions.setFinalScriptsBlockedState()
    const allOrigins = Object.keys(this.noScriptInfo)
    const allNonBlockedOrigins = allOrigins.filter(key => allOrigins[key].willBlock === false)
    this.noScriptActions.allowScriptOriginsOnce(allNonBlockedOrigins)
  }

  getBlockAllText (shouldBlock: boolean) {
    return getBlockAllText(this.noScriptInfo, shouldBlock)
  }

  render () {
    const { favicon, hostname } = this.props
    return (
      <BlockedListContent>
        <BlockedListHeader>
          <Favicon src={favicon} />
          <SiteInfoText>{hostname}</SiteInfoText>
        </BlockedListHeader>
        <details open={true}>
          <BlockedListSummary stats={false}>
            <ArrowUpIcon />
            <BlockedListSummaryText>{getLocale('scriptsBlocked')}</BlockedListSummaryText>
          </BlockedListSummary>
          <BlockedListDynamic>
          {
            this.blockedScriptsLength > 0 && (
              <>
                <BlockedListItemHeader id='blocked'>
                  <BlockedListItemHeaderStats>
                    {this.blockedScriptsLength}
                  </BlockedListItemHeaderStats>
                  <BlockedListItemHeaderText>
                      {getLocale('blockedScripts')}
                  </BlockedListItemHeaderText>
                  <LinkAction
                    onClick={this.blockOrAllowAll.bind(this, false)}
                  >
                    {this.getBlockAllText(true)}
                  </LinkAction>
                </BlockedListItemHeader>
                <NoScriptContent
                  shouldBlock={true}
                  noScriptActions={this.noScriptActions}
                  noScriptInfo={this.generatedNoScriptData}
                />
              </>
            )
          }
          {
            this.allowedScriptsLength > 0 && (
              <>
                <BlockedListItemHeader id='allowed'>
                  <BlockedListItemHeaderStats>
                    {this.allowedScriptsLength}
                  </BlockedListItemHeaderStats>
                  <BlockedListItemHeaderText>
                      {getLocale('allowedScripts')}
                  </BlockedListItemHeaderText>
                  <LinkAction
                    onClick={this.blockOrAllowAll.bind(this, true)}
                  >
                    {this.getBlockAllText(false)}
                  </LinkAction>
                </BlockedListItemHeader>
                <NoScriptContent
                  shouldBlock={false}
                  noScriptActions={this.noScriptActions}
                  noScriptInfo={this.generatedNoScriptData}
                />
              </>
            )
          }
          </BlockedListDynamic>
        </details>
        <BlockedListFooter>
          <ShieldsButton
            level='primary'
            type='accent'
            onClick={this.setFinalScriptsBlockedState}
            text={getLocale('goBack')}
          />
        </BlockedListFooter>
      </BlockedListContent>
    )
  }
}
