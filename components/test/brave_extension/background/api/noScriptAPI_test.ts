/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

import * as noScriptAPI from '../../../../brave_extension/extension/brave_extension/background/api/noScriptAPI'

describe('NoScript API', () => {
  describe('setAllowJavaScript', () => {
    let spy: jest.SpyInstance
    beforeEach(() => {
      spy = jest.spyOn(chrome.braveShields.javascript, 'setAsync')
    })
    afterEach(() => {
      spy.mockRestore()
    })

    it('calls chrome.braveShields.plugins with the correct args', () => {
      noScriptAPI.setAllowJavaScript('https://www.brave.com', 'block')
        .catch(() => {
          expect(true).toBe(false)
        })
      const arg0 = spy.mock.calls[0][0]
      expect.assertions(1)
      expect(arg0).toEqual({
        primaryPattern: 'https://www.brave.com/*',
        setting: 'block',
        scope: 'incognito_session_only'
      })
    })

    it('passes only 1 arg to chrome.braveShields.plugins', () => {
      noScriptAPI.setAllowJavaScript('https://www.brave.com', 'block')
        .catch(() => {
          expect(true).toBe(false)
        })
      expect.assertions(1)
      expect(spy.mock.calls[0].length).toBe(1)
    })

    it('resolves the returned promise', (cb) => {
      noScriptAPI.setAllowJavaScript('https://www.brave.com', 'block')
        .then(cb)
        .catch((e: Error) => {
          console.error(e.toString())
        })
    })
  })

  describe('setAllowScriptOriginsOnce', () => {
    let spy: jest.SpyInstance
    beforeEach(() => {
      spy = jest.spyOn(chrome.braveShields, 'allowScriptsOnce')
    })
    afterEach(() => {
      spy.mockRestore()
    })
    it('calls chrome.braveShields.allowScriptsOnce with the correct args', () => {
      noScriptAPI.setAllowScriptOriginsOnce(['https://a.com/', 'https://b.com/'], 2)
        .catch(() => {
          expect(true).toBe(false)
        })
      const arg0 = spy.mock.calls[0][0]
      expect.assertions(2)
      expect(arg0).toEqual(['https://a.com/', 'https://b.com/'])
      const arg1 = spy.mock.calls[0][1]
      expect(arg1).toBe(2)
    })
    it('passes 3 args to chrome.braveShields.allowScriptsOnce', () => {
      noScriptAPI.setAllowScriptOriginsOnce(['https://a.com/', 'https://b.com/'], 2)
        .catch(() => {
          expect(true).toBe(false)
        })
      expect.assertions(1)
      expect(spy.mock.calls[0].length).toBe(3) // include callback
    })
    it('resolves the returned promise', (cb) => {
      noScriptAPI.setAllowScriptOriginsOnce(['https://a.com/', 'https://b.com/'], 2)
      .then(() => {
        cb()
      })
      .catch((e: Error) => {
        console.error(e.toString())
      })
    })
  })
})
