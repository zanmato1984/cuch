#include <Columns/ColumnsNumber.h>
#include <Core/Block.h>
#include <Cudf/CudfJoin.h>

#include <numeric>

namespace DB
{
bool CudfJoin::addJoinedBlock(const Block & block)
{
    right_blocks.push_back(block);
    return true;
}

void CudfJoin::joinBlock(Block & block)
{
    if (!right_table)
        return;

    std::unique_ptr<cudf::table> left_table;
    {
        auto column = std::make_unique<cudf::column>(
            cudf::data_type{cudf::type_id::INT32},
            block.rows(),
            rmm::device_buffer{block.rows() * cudf::size_of(cudf::data_type{cudf::type_id::INT32})});
        std::vector<std::unique_ptr<cudf::column>> columns;
        columns.push_back(std::move(column));
        left_table = std::make_unique<cudf::table>(std::move(columns));
        auto mv = left_table->mutable_view();
        RMM_CUDA_TRY(cudaMemcpy(
            mv.column(0).data<char>(),
            block.getByPosition(0).column->getRawData().data,
            block.rows() * sizeof(int32_t),
            cudaMemcpyDefault));
    }

    auto joined = hash_join->inner_join(*left_table, {0}, {{0, 0}});
    auto v = joined->view();
    {
        auto & column = block.getByPosition(0);
        auto col = ColumnInt32::create();
        auto & vec = col->getData();
        vec.resize(v.num_rows());
        RMM_CUDA_TRY(
            cudaMemcpy(reinterpret_cast<void *>(vec.data()), v.column(0).data<void>(), v.num_rows() * sizeof(int32_t), cudaMemcpyDefault));
        column.column = std::move(col);
    }
}

bool CudfJoin::hasTotals() const
{
    return false;
}

void CudfJoin::setTotals(const Block & block)
{
    (void)block;
}

void CudfJoin::joinTotals(Block & block) const
{
    (void)block;
}

size_t CudfJoin::getTotalRowCount() const
{
    return 0;
}

void CudfJoin::finishInsert()
{
    size_t rows
        = std::accumulate(right_blocks.begin(), right_blocks.end(), 0, [&](size_t sum, const auto & block) { return sum + block.rows(); });
    {
        auto column = std::make_unique<cudf::column>(
            cudf::data_type{cudf::type_id::INT32}, rows, rmm::device_buffer{rows * cudf::size_of(cudf::data_type{cudf::type_id::INT32})});
        std::vector<std::unique_ptr<cudf::column>> columns;
        columns.push_back(std::move(column));
        right_table = std::make_unique<cudf::table>(std::move(columns));
    }
    auto mv = right_table->mutable_view();
    size_t offset = 0;
    for (const auto & block : right_blocks)
    {
        RMM_CUDA_TRY(cudaMemcpy(
            mv.column(0).data<char>() + offset,
            block.getByPosition(0).column->getRawData().data,
            block.rows() * sizeof(int32_t),
            cudaMemcpyDefault));
        offset += block.rows() * sizeof(int32_t);
    }
    hash_join = cudf::hash_join::create(*right_table, {0});
    {
        BlocksList blocks;
        std::swap(blocks, right_blocks);
    }
}

}
