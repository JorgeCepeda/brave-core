/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/ai_chat/core/browser/model_service_factory.h"

#include "base/no_destructor.h"
#include "brave/components/ai_chat/core/browser/model_service.h"
#include "brave/components/ai_chat/core/common/features.h"
#include "chrome/browser/profiles/profile.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"

namespace ai_chat {

// static
ModelServiceFactory* ModelServiceFactory::GetInstance() {
  static base::NoDestructor<ModelServiceFactory> instance;
  return instance.get();
}

// static
ModelService* ModelServiceFactory::GetForBrowserContext(
    content::BrowserContext* context) {
  DCHECK(context);
  if (features::IsAIChatEnabled()) {
    return static_cast<ModelService*>(
        GetInstance()->GetServiceForBrowserContext(context, true));
  }

  return nullptr;
}

ModelServiceFactory::ModelServiceFactory()
    : BrowserContextKeyedServiceFactory(
          "ModelServiceFactory",
          BrowserContextDependencyManager::GetInstance()) {}

ModelServiceFactory::~ModelServiceFactory() = default;

std::unique_ptr<KeyedService>
ModelServiceFactory::BuildServiceInstanceForBrowserContext(
    content::BrowserContext* context) const {
  return std::make_unique<ModelService>(
      Profile::FromBrowserContext(context)->GetPrefs());
}

}  // namespace ai_chat
