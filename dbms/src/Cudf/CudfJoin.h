#pragma once

#include <shared_mutex>
#include <Interpreters/IJoin.h>

#include <cudf/column/column.hpp>
#include <cudf/join.hpp>
#include <cudf/table/table.hpp>


namespace DB
{
class CudfJoin : public IJoin
{
public:
    bool addJoinedBlock(const Block & block) override;
    void joinBlock(Block & block) override;
    bool hasTotals() const override;
    void setTotals(const Block & block) override;
    void joinTotals(Block & block) const override;
    size_t getTotalRowCount() const override;
    void finishInsert() override;

private:
    BlocksList right_blocks;
    std::unique_ptr<cudf::table> right_table;
    std::unique_ptr<const cudf::hash_join> hash_join;
};

}
