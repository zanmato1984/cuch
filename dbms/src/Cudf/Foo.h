#pragma once

#include <cudf/column/column.hpp>
#include <DataStreams/DistinctBlockInputStream.h>

namespace DB
{
    class Foo
    {
    public:
        Foo();
    private:
        cudf::column column;
    };
}