#define LOCAL
#ifdef LOCAL
    auto start = chrono::steady_clock::now();
#endif

#ifdef LOCAL
    auto finish = chrono::steady_clock::now();
    cerr << fixed << setprecision(3);
    cerr << "Time: "
         << chrono::duration<double, milli>(finish - start).count()
         << " ms\n";
#endif