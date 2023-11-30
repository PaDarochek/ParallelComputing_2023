#include <iostream>
#include <vector>
#include <cstdint>

static std::vector<std::pair<size_t, size_t>> comparators;

void add_comp(size_t idx1, size_t idx2)
{
    comparators.push_back(std::make_pair(idx1, idx2));
}

void merge_lines(size_t idx1, size_t idx2, size_t step, size_t count1, size_t count2)
{
    if (count1 * count2 < 1)
    {
        return;
    }
    if (count1 == 1 && count2 == 1)
    {
        add_comp(idx1, idx2);
        return;
    }

    size_t count11 = count1 - count1 / 2;
    size_t count22 = count2 - count2 / 2;
    merge_lines(idx1, idx2, 2 * step, count11, count22);
    merge_lines(idx1 + step, idx2 + step, 2 * step, count1 - count11, count2 - count22);

    size_t i;
    for (i = 1; i < count1 - 1; i += 2)
    {
        add_comp(idx1 + step * i, idx1 + step * (i + 1));
    }

    if (count1 % 2 == 0)
    {
        add_comp(idx1 + step * (count1 - 1), idx2);
        i = 1;
    }
    else
    {
        i = 0;
    }

    for (; i < count2 - 1; i += 2)
    {
        add_comp(idx2 + step * i, idx2 + step * (i + 1));
    }

}

void build_net(size_t idx, size_t step, size_t count)
{
    if (count < 2)
    {
        return;
    }

    if (count == 2)
    {
        add_comp(idx, idx + step);
        return;
    }

    size_t count1 = count / 2 + count % 2;
    build_net(idx, step, count1);
    build_net(idx + step * count1, step, count - count1);

    merge_lines(idx, idx + step * count1, step, count1, count - count1);
}

size_t count_tacts(size_t count)
{
    std::vector<size_t> comp_execution_tacts(count, 0);
    size_t tacts = 0;
    for (int32_t i = 0; i < comparators.size(); ++i)
    {
        size_t first = comparators[i].first;
        size_t second = comparators[i].second;
        size_t exec_tact = std::max(comp_execution_tacts[first], comp_execution_tacts[second]) + 1;
        comp_execution_tacts[first] = comp_execution_tacts[second] = exec_tact;
        if (exec_tact > tacts)
        {
            tacts = exec_tact;
        }
    }
    return tacts;
}

void compare(std::vector<int32_t>& data, std::pair<size_t, size_t> comparator)
{
    size_t first = comparator.first;
    size_t second = comparator.second;
    if (data[first] > data[second])
    {
        std::swap(data[first], data[second]);
    }
}

void check_sort(size_t count)
{
    if (count > 24)
    {
        return;
    }
    std::vector<int32_t> data(count);
    for (int32_t i = 0; i < 1 << count; ++i)
    {
        int32_t num = i;
        for (int32_t j = count - 1; j >= 0; --j)
        {
            data[j] = num & 1;
            num >>= 1;
        }

        for (size_t j = 0; j < comparators.size(); ++j)
        {
            compare(data, comparators[j]);
        }
        for (size_t j = 0; j < count - 1; ++j)
        {
            if (data[j] > data[j + 1])
            {
                std::cout << "Test failed for " << i << std::endl;
                return;
            }
        }
    }
    std::cout << "Tests passed!" << std::endl;

}

int main(int argc, char *argv[])
{
    size_t count;
    if (argc < 2)
    {
        return 1;
    }
    count = std::stoi(argv[1]);

    build_net(0, 1, count);

    std::cout << count << " 0 0" << std::endl; 
    for (const auto &p : comparators)
    {
        std::cout << p.first << " " << p.second << std::endl;
    }
    std::cout << comparators.size() << std::endl;
    std::cout << count_tacts(count) << std::endl;

    check_sort(count);
    return 0;
}
