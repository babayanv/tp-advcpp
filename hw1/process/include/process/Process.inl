struct BadProcess;


template<class DataType, class>
size_t Process::write(const DataType& data)
{
    static constexpr size_t data_size = sizeof(DataType);

    writeExact(&data, data_size);

    return data_size;
}


template<class DataType, class>
size_t Process::read(DataType& data)
{
    static constexpr size_t data_size = sizeof(DataType);

    readExact(&data, data_size);

    return data_size;
}
