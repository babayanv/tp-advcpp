struct BadProcess;


template<class DataType, class>
size_t Process::write(const DataType* data)
{
    ssize_t bytes_written = ::write(m_p2c_fd, data, sizeof(DataType));

    if (bytes_written == WRITE_FAILED)
    {
        throw BadProcess();
    }

    return bytes_written;
}


template<class DataType, class>
void Process::writeExact(const DataType* data)
{
    size_t bytes_written = 0;

    static constexpr size_t len = sizeof(DataType);

    while (bytes_written != len)
    {
        const void* buff_begin = reinterpret_cast<const char*>(data) + bytes_written;

        bytes_written += write(buff_begin, len - bytes_written);
    }
}


template<class DataType, class>
size_t Process::read(DataType* data)
{
    ssize_t bytes_read = ::read(m_c2p_fd, data, sizeof(DataType));

    if (bytes_read == READ_FAILED)
    {
        throw BadProcess();
    }

    if (bytes_read == READ_EOF)
    {
        m_is_readable = false;
    }

    return bytes_read;
}


template<class DataType, class>
void Process::readExact(DataType* data)
{
    size_t bytes_read = 0;

    static constexpr size_t len = sizeof(DataType);

    while (bytes_read != len)
    {
        void* buff_begin = reinterpret_cast<char*>(data) + bytes_read;

        bytes_read += read(buff_begin, len - bytes_read);
    }
}
