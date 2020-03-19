struct BadProcess;


template<class DataType, class>
void Process::write(const DataType& data)
{
    writeExact(&data, sizeof(DataType));
}


template<class DataType, class>
void Process::read(DataType& data)
{
    readExact(&data, sizeof(DataType));
}
