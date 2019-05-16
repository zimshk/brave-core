/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_ADS_NOTIFICATION_VIEW_H_
#define BRAVE_BROWSER_UI_VIEWS_ADS_NOTIFICATION_VIEW_H_

#include "content/public/browser/web_contents_observer.h"
#include "ui/views/widget/widget_delegate.h"

class Browser;

class AdsNotificationView : public views::WidgetDelegateView {
 public:
  static void Show(Browser* browser,
                   const GURL& url,
                   const gfx::Rect& bounds = gfx::Rect());

  AdsNotificationView(Browser* browser, const GURL& url);
  ~AdsNotificationView() override;

 private:
  void Close();

  DISALLOW_COPY_AND_ASSIGN(AdsNotificationView);
};

#endif  // BRAVE_BROWSER_UI_VIEWS_ADS_NOTIFICATION_VIEW_H_
