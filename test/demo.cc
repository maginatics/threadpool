//
// Copyright (c) Maginatics, Inc. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdio.h>
#include <inttypes.h>

#include <boost/atomic.hpp>

#include <maginatics/threadpool/threadpool.h>

//
// A trivial demonstration of the thread pool interfaces
//

int main(int argc, char **argv) {
    const int64_t kMinSize = 1;
    const int64_t kMaxSize = 10;
    const int64_t kKeepAliveMs = 5000;

    maginatics::ThreadPool pool(kMinSize, kMaxSize, kKeepAliveMs);

    // Executing tasks on the pool
    printf("Counting to one billion in parallel... ");
    fflush(stdout);
    boost::atomic<int64_t> val(0);
    for (int i = 0; i < 1000; ++i) {
        pool.execute([&]() {
                for (int i = 0; i < 1E6; ++i) {
                    ++val;
                }
            });
    }
    pool.drain();
    printf("sum: %" PRId64"\n", val.load());

    // Scheduling tasks that return values
    printf("Scheduling a result-generating task... ");
    fflush(stdout);
    auto result = pool.schedule<int64_t>([]() -> int64_t {
                volatile int64_t ret = 0;
                for (int i = 0; i < 1E6; ++i) {
                    ++ret;
                }
                return ret;
            });
    printf("result: %" PRId64"\n", result.get());

    return 0;
}
