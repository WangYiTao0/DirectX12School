#include "CTexture.h"
#include "renderer.h"


void CTexture::load(const char* FileName)
{
	unsigned char header[18];
	std::vector<char> image;
	unsigned int width, height;
	unsigned char depth;
	unsigned int bpp;
	unsigned int size;

	std::ifstream file(FileName, std::ios_base::in | std::ios_base::binary);

	assert(file);

	file.read((char*)header, sizeof(header));

	width = header[13] * 256 + header[12];
	height = header[15] * 256 + header[14];
	depth = header[16];

	if (depth == 32)
			bpp = 4;
		else if (depth == 24)
			bpp = 3;
		else
			bpp = 0;

	if (bpp != 4)
	{
		assert(false);
	}
	size = width * height * bpp;

	image.resize(size);

	file.read(&image[0], size);

	file.close();

	ComPtr<ID3D12Device> device = CRenderer::GetInstance()->GetDevice();


	HRESULT hr{};

	D3D12_HEAP_PROPERTIES heap_properties{};
	heap_properties.CreationNodeMask = 0;
	heap_properties.VisibleNodeMask = 0;
	heap_properties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heap_properties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heap_properties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC resource_desc{};
	resource_desc.DepthOrArraySize = 1;
	resource_desc.MipLevels = 1;
	resource_desc.SampleDesc.Count = 1;
	resource_desc.SampleDesc.Quality = 0;
	resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resource_desc.Width = width;
	resource_desc.Height = height;
	resource_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resource_desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

	hr = device->CreateCommittedResource(&heap_properties, D3D12_HEAP_FLAG_NONE, &resource_desc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,IID_PPV_ARGS(&m_Resource));
	assert(SUCCEEDED(hr));

	D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc{};
	descriptor_heap_desc.NumDescriptors = 1;
	descriptor_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descriptor_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descriptor_heap_desc.NodeMask = 0;
	hr = device->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&m_DescriptorHeap));
	assert(SUCCEEDED(hr));

	D3D12_CPU_DESCRIPTOR_HANDLE handle_srv{};
	D3D12_SHADER_RESOURCE_VIEW_DESC resourct_view_desc{};

	resourct_view_desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	resourct_view_desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	resourct_view_desc.Texture2D.MipLevels = 1;
	resourct_view_desc.Texture2D.MostDetailedMip = 0;
	resourct_view_desc.Texture2D.PlaneSlice = 0;
	resourct_view_desc.Texture2D.ResourceMinLODClamp = 0.0F;
	resourct_view_desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	handle_srv = m_DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	device->CreateShaderResourceView(*m_Resource.GetAddressOf(), &resourct_view_desc, handle_srv);

	D3D12_BOX box = { 0,0,0, (UINT)width,(UINT)height,1 };
	hr = m_Resource->WriteToSubresource(0, &box, &image[0], 4 * width, 4 * height * width);
	assert(SUCCEEDED(hr));
}
