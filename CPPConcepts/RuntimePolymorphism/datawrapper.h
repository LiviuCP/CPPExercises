#pragma once

#include <cassert>
#include <fstream>
#include <memory>
#include <string>

/* These 2 methods should be defined for each data type if a specific behavior is required:
   - either as template specialization
   - or as free function having the specific data type as argument (either way it is acceptable)
*/
template <typename DataType> void display(const DataType& data, std::ofstream& out, size_t indentation);

template <typename DataType> size_t countContent(const DataType&);

/* A compact implementation that allows implementing a unitary behavior to virtually any data type
   The only requirement is to define the above methods for that specific data type if the default implementation is not
   wanted or is not applicable to the specific object (no stream << operator defined)

   Example for a class MyObject:

   void display(const MyObject& data, std::ofstream& out, size_t indentation)
   {
       // display object
   }

   size_t countContent(const MyObject& data)
   {
       // count the object's content
   }
*/
class DataWrapper
{
public:
    template <typename DataType> DataWrapper(DataType data);

    DataWrapper(const DataWrapper& wrapper);
    DataWrapper(DataWrapper&&) noexcept = default;

    DataWrapper& operator=(const DataWrapper& data);
    DataWrapper& operator=(DataWrapper&&) noexcept = default;

    friend void displayData(const DataWrapper& wrapper, std::ofstream& out, size_t indentation);
    friend size_t countDataContent(const DataWrapper& wrapper);

private:
    struct AbstractModel
    {
        virtual ~AbstractModel() = default;
        virtual std::unique_ptr<AbstractModel> copy() const = 0;
        virtual void displayModel(std::ofstream&, size_t) const = 0;
        virtual size_t countModelContent() const = 0;
    };

    template <typename DataType> struct ConcreteModel final : AbstractModel
    {
        ConcreteModel(DataType data);

        std::unique_ptr<AbstractModel> copy() const override;
        void displayModel(std::ofstream& out, size_t indentation) const override;
        size_t countModelContent() const override;

        DataType mData;
    };

    std::unique_ptr<AbstractModel> m_pModel;
};

template <typename DataType> void display(const DataType& data, std::ofstream& out, size_t indentation)
{
    if (out.is_open())
    {
        out << std::string(indentation, ' ') << data << std::endl;
    }
    else
    {
        assert(false);
    }
}

/* Some of the objects might be counted as more than 1 (e.g. a std::pair might be counted as 2)
   This should be explicitly stated (either by using a template specialization or in a free function with the respective
   data type as argument)
*/
template <typename DataType> size_t countContent(const DataType&)
{
    return 1;
}

template <typename DataType>
DataWrapper::DataWrapper(DataType data)
    : m_pModel(std::make_unique<ConcreteModel<DataType>>(std::move(data)))
{
}

inline DataWrapper::DataWrapper(const DataWrapper& wrapper)
    : m_pModel(wrapper.m_pModel->copy())
{
}

inline DataWrapper& DataWrapper::operator=(const DataWrapper& data)
{
    return *this = DataWrapper(data);
}

inline void displayData(const DataWrapper& wrapper, std::ofstream& out, size_t indentation)
{
    wrapper.m_pModel->displayModel(out, indentation);
}

inline size_t countDataContent(const DataWrapper& wrapper)
{
    return wrapper.m_pModel->countModelContent();
}

template <typename DataType>
DataWrapper::ConcreteModel<DataType>::ConcreteModel(DataType data)
    : mData(std::move(data))
{
}

template <typename DataType>
std::unique_ptr<DataWrapper::AbstractModel> DataWrapper::ConcreteModel<DataType>::copy() const
{
    return std::make_unique<ConcreteModel>(*this);
}

template <typename DataType>
void DataWrapper::ConcreteModel<DataType>::displayModel(std::ofstream& out, size_t indentation) const
{
    display(mData, out, indentation);
}

template <typename DataType> size_t DataWrapper::ConcreteModel<DataType>::countModelContent() const
{
    return countContent(mData);
}
