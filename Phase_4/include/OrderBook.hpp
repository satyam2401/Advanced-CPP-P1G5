template <typename PriceType, typename OrderIdType>
struct Order {
    OrderIdType id;
    PriceType price;
    int quantity;
    bool is_buy;
};