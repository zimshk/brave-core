/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/ads_notification_view.h"

#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/views/controls/webview/web_contents_set_background_color.h"
#include "ui/views/controls/webview/webview.h"
#include "ui/views/layout/fill_layout.h"
#include "url/gurl.h"

namespace {
AdsNotificationView* g_active_ads_window = nullptr;
}  // namespace

// static
void AdsNotificationView::Show(Browser* browser,
                               const GURL& url,
                               const gfx::Rect& rect) {
  if (g_active_ads_window)
    g_active_ads_window->Close();

  views::Widget* window = new views::Widget;
  views::Widget::InitParams params;
  params.ownership = views::Widget::InitParams::WIDGET_OWNS_NATIVE_WIDGET;
  // params.parent = BrowserView::GetBrowserViewForBrowser(browser)->GetWidget()->GetNativeView();
  params.bounds = { 0, 0, 350, 600 };
  g_active_ads_window = new AdsNotificationView(browser, url);
  params.delegate = g_active_ads_window;
  params.type = views::Widget::InitParams::TYPE_WINDOW_FRAMELESS;
  params.opacity = views::Widget::InitParams::TRANSLUCENT_WINDOW;
  params.shadow_type = views::Widget::InitParams::SHADOW_TYPE_NONE;

  window->Init(params);

  window->CenterWindow(params.bounds.size());
  window->ShowInactive();
}

AdsNotificationView::~AdsNotificationView() {
  LOG(ERROR) << __FUNCTION__;
}

AdsNotificationView::AdsNotificationView(Browser* browser, const GURL& url) {
  // Life cycle control
  //  * When this is hidden?
  //  * This can
  //  * Ads windows should be
  // Which profiles should be used for this webview?
  // Profile can be removed during the runtime.
  // This window should be destroyed when |browser->profile()| is closed/removed?
  // Also for browser?
  // auto* profile = browser->profile();
  // registrar_.Add(this, chrome::NOTIFICATION_PROFILE_DESTROYED,
  //                  content::Source<Profile>(profile_));
  // How to know ads notification window is visible or not?
  // This could be happen when multiple profiles are used.

  auto* web_view = new views::WebView(browser->profile());
  views::WebContentsSetBackgroundColor::CreateForWebContentsWithColor(
      web_view->GetWebContents(),
      SK_ColorTRANSPARENT);
  web_view->LoadInitialURL(url);
  SetLayoutManager(std::make_unique<views::FillLayout>());
  AddChildView(web_view);
}

void AdsNotificationView::Close() {
  GetWidget()->Close();
}
