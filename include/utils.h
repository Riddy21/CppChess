// Custom hash function for std::array
struct ArrayHash {
    template <typename T, std::size_t N>
    std::size_t operator()(const std::array<T, N>& arr) const {
        std::hash<T> hasher;
        std::size_t seed = 0;
        for (const T& elem : arr) {
            seed ^= hasher(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};