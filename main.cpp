#include <exception>
#include <iostream>
#include <unordered_map>

template<class Key, class Value>
class dictionary
{
public:
    virtual ~dictionary() noexcept = default;

    virtual const Value& get(const Key& key) const = 0;
    virtual void set(const Key& key, const Value& value) = 0;
    virtual bool is_set(const Key& key) const = 0;
};

template<class Key>
class not_found_exception : public std::exception
{
public:
    [[nodiscard]] virtual const Key& get_key() const noexcept = 0;
};

template <class Key>
class NotFoundExceptionImpl : public not_found_exception<Key>
{
public:
    explicit NotFoundExceptionImpl(Key key) : _key(std::move(key)) { }

    const Key& get_key() const noexcept override
    {
        return _key;
    }

    const char* what() const noexcept override
    {
        return "Key not found";
    }

private:
    Key _key;
};

template <class Key, class Value>
class DictionaryImpl : public dictionary<Key, Value>
{
public:
    DictionaryImpl() = default;
    ~DictionaryImpl() noexcept override = default;

    const Value& get(const Key& key) const override
    {
        auto itr = _store.find(key);
        if (itr == _store.end())
            throw NotFoundExceptionImpl<Key>(key);

        return itr->second;
    }

    void set(const Key& key, const Value& value) override
    {
        auto itr = _store.find(key);
        if (itr != _store.end())
        {
            itr->second = value;
            return;
        }

        _store.emplace(key, value);
    }

    bool is_set(const Key& key) const override
    {
        return _store.find(key) != _store.end();
    }

private:
    std::unordered_map<Key, Value> _store;
};

int main()
{
    using KeyType = int32_t;
    using ValueType = int32_t;

    try
    {
        DictionaryImpl<KeyType, ValueType> dictionary;

        // Add keys
        dictionary.set(1, 1);
        dictionary.set(2, 1);
        dictionary.set(3, 1);

        // Check is set keys
        std::cout << "> Is set 1? " << std::boolalpha << dictionary.is_set(1) << std::endl;
        std::cout << "> Is set 2? " << std::boolalpha << dictionary.is_set(2) << std::endl;
        std::cout << "> Is set 3? " << std::boolalpha << dictionary.is_set(3) << std::endl;
        std::cout << "> Is set 4? " << std::boolalpha << dictionary.is_set(4) << std::endl;

        // Get value from keys
        std::cout << "> Key 1: " << dictionary.get(1) << std::endl;
        std::cout << "> Key 2: " << dictionary.get(2) << std::endl;
        std::cout << "> Key 3: " << dictionary.get(3) << std::endl;
        std::cout << "> Key 4: " << dictionary.get(4) << std::endl;
    }
    catch (const NotFoundExceptionImpl<KeyType>& e)
    {
        std::cout << "\n\n" << e.what() << ". Key: " << e.get_key() << std::endl;
    }

    return 0;
}
