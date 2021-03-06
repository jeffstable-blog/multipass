/*
 * Copyright (C) 2019 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "process_factory.h"
#include "linux_process.h"
#include "process_spec.h"

namespace mp = multipass;

namespace
{
class UnsecuredProcess : public mp::LinuxProcess
{
public:
    UnsecuredProcess(std::unique_ptr<mp::ProcessSpec>&& spec) : LinuxProcess{std::move(spec)}
    {
    }
};

class SimpleProcessSpec : public mp::ProcessSpec
{
public:
    SimpleProcessSpec(const QString& cmd, const QStringList& args) : cmd{cmd}, args{args}
    {
    }

    QString program() const override
    {
        return cmd;
    }
    QStringList arguments() const override
    {
        return args;
    }

private:
    const QString cmd;
    const QStringList args;
};
} // namespace

mp::ProcessFactory::ProcessFactory(const Singleton<ProcessFactory>::PrivatePass& pass)
    : Singleton<ProcessFactory>::Singleton{pass}
{
}

// This is the default ProcessFactory that creates a Process with no security mechanisms enabled
std::unique_ptr<mp::Process> mp::ProcessFactory::create_process(std::unique_ptr<mp::ProcessSpec>&& process_spec) const
{
    return std::make_unique<UnsecuredProcess>(std::move(process_spec));
}

std::unique_ptr<mp::Process> mp::ProcessFactory::create_process(const QString& command,
                                                                const QStringList& arguments) const
{
    return create_process(std::make_unique<SimpleProcessSpec>(command, arguments));
}
