
#include"UploadBuffer.h"
#include "Helpers.h"


UploadBuffer::Allocation UploadBuffer::Allocate(size_t sizeInBytes, size_t alignment) 
{
	if (sizeInBytes > m_PageSize)
	{
		throw std::bad_alloc(); 
	}

	if (!m_CurrentPage || !m_CurrentPage->HasSpace(sizeInBytes, alignment))
	{
		m_CurrentPage = RequestPage();
	}
	return m_CurrentPage->Allocate(sizeInBytes, alignment);
}


std::shared_ptr<UploadBuffer::Page> UploadBuffer::RequestPage()
{
	std::shared_ptr<Page> page; 
	if (!m_AvailablePages.empty())
	{
		page = m_AvailablePages.front(); 
		m_AvailablePages.pop_front(); 
	}
	else
	{
		page = std::make_shared<Page>(m_PageSize);
		m_PagePool.push_back(page);
	}
	return page; 
}



UploadBuffer::Page::Page(size_t sizeInBytes) :
	m_PageSize(sizeInBytes),
	m_Offset(0),
	m_CPUPtr(nullptr),
	m_GPUPtr(D3D12_GPU_VIRTUAL_ADDRESS(0))
{
	auto device = Application::Get().GetDevice();

}

UploadBuffer::Allocation UploadBuffer::Page::Allocate(size_t sizeInBytes, size_t alignment)
{
	if (!HasSpace(sizeInBytes, alignment))
	{
		throw std::bad_alloc(); 
	}

	size_t alignedSize = Math::AlignUp(sizeInBytes, alignment);
	m_Offset = Math::AlignUp(m_Offset, alignment); 

	Allocation allocation; 
	allocation.CPU = static_cast<uint8_t*>(m_CPUPtr) + m_Offset; 
	allocation.GPU = m_GPUPtr + m_Offset; 

	m_Offset += alignedSize; 
	return allocation; 
}


void UploadBuffer::Page::Reset()
{
	m_Offset = 0; 
}