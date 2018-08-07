/****************************************************************************
**
** Copyright (C) 2018 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the WebP plugins in the Qt ImageFormats module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "contextwriter_p.h"

#include <type_traits>

ContextWriter::ContextWriter(QIODevice& device) :
    _device(device)
{
}

heif_error ContextWriter::write(const void* data, size_t size)
{
    qint64 bytesWritten = _device.write(static_cast<const char*>(data), size);

    using I = typename std::conditional<sizeof(size_t) >= sizeof(qint64),
                                        size_t,
                                        qint64>::type;

    if (bytesWritten < 0 || static_cast<I>(bytesWritten) != static_cast<I>(size)) {
        qWarning("ContextWriter::write() write failed: %lld / %zu bytes written", bytesWritten, size);

        return {
            heif_error_Encoding_error,
            heif_suberror_Cannot_write_output_data,
            "write failed"
        };
    }

    return {
        heif_error_Ok,
        heif_suberror_Unspecified,
        "ok"
    };
}
