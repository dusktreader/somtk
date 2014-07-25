#pragma once

#include <QString>
#include "local_assert.h"

#define ASSERT_QMSG( expr, msg ) ASSERT_MSG( expr, msg.toStdString() )
