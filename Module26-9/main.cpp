//
//  main.cpp
//  Module26-9
//
//  Created by Ольга Полевик on 13.12.2021.
//

#include <iostream>
#include <vector>
#include <thread>
#include <array>
#include <chrono>

int main()
{
    const size_t N = 10000000;
    const size_t THREADS = 4;
    
    std::vector<int> data;
    for ( int i = 0; i < N; i++)
    {
        data.push_back( rand() % 10 );
    }

    auto single_thread_start = std::chrono::steady_clock::now();
    
    int64_t control_sum = 0;
    
    for ( int i = 0; i < N; i++)
    {
        control_sum += data[i];
    }
    
    auto single_thread_end = std::chrono::steady_clock::now();
    
    std::vector<std::thread> threads;
    std::array<int64_t, THREADS> sums;
    const size_t INDEXES_PER_THREAD = N / THREADS;

    size_t first_index = 0;
    
    auto multi_thread_start = std::chrono::steady_clock::now();
    
    for ( size_t thread_index = 0; thread_index < THREADS; ++thread_index )
    {
        size_t last_index = first_index + INDEXES_PER_THREAD;
        if ( last_index >= data.size() )
            last_index = data.size();

        threads.emplace_back([&data, &sums, thread_index, first_index, last_index]()
        {
            int64_t sum = 0;
            
            for ( size_t i = first_index; i < last_index; ++i )
            {
                sum += data[i];
            }
            
            sums[thread_index] = sum;

        });
        
        first_index = last_index;
    }
    
    int64_t total_sum = 0;
    
    for ( size_t thread_index = 0; thread_index < THREADS; ++thread_index )
    {
        threads[thread_index].join();
        total_sum += sums[thread_index];
    }
    
    auto multi_thread_end = std::chrono::steady_clock::now();

    std::chrono::duration<double> multi_time = ( multi_thread_end - multi_thread_start );
    std::chrono::duration<double> single_time = ( single_thread_end - single_thread_start );

    std::cout << "multi : " << total_sum << ", " << multi_time.count() * 1000. << " [ms]" << std::endl;
    std::cout << "single: " << control_sum << ", " << single_time.count() * 1000. << " [ms]" <<  std::endl;
    
    return 0;
}
