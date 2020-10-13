#include "CPolygonDeferred.h"
#include "main.h"
#include "renderer.h"




void CPolygonDeferred::Initialized()
{
	ComPtr<ID3D12Device> device = CRenderer::GetInstance()->GetDevice();

	if (device == nullptr) return;

	HRESULT hr;
	D3D12_HEAP_PROPERTIES heapPropertes{};
	D3D12_RESOURCE_DESC resourceDesc{};

	heapPropertes.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapPropertes.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapPropertes.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapPropertes.CreationNodeMask = 0;
	heapPropertes.VisibleNodeMask = 0;

	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;

	//頂点バッファ
	resourceDesc.Width = sizeof(Vertex3D) * 4;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_VertexBuffer));
	ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	//定数バッファ
	resourceDesc.Width = 256;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_ConstantBuffer));
	ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	//頂点データの書き込み
	Vertex3D* buffer{};
	hr = m_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));
	ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	buffer[0].Position = { 0.0,0.0,0.0 };
	buffer[1].Position = { 320.0,0.0,0.0 };
	buffer[2].Position = { 0.0,240.0,0.0 };
	buffer[3].Position = { 320,240.0,0.0 };
	buffer[0].Normal = { 0.0,1.0,0.0 };
	buffer[1].Normal = { 0.0,1.0,0.0 };
	buffer[2].Normal = { 0.0,1.0,0.0 };
	buffer[3].Normal = { 0.0,1.0,0.0 };
	buffer[0].TexCoord = { 0.0,0.0 };
	buffer[1].TexCoord = { 1.0,0.0 };
	buffer[2].TexCoord = { 0.0,1.0 };
	buffer[3].TexCoord = { 1.0,1.0 };

	m_VertexBuffer->Unmap(0, nullptr);

	//m_texture = std::make_unique<CTexture>();
	//m_texture->load("data/field004.tga");
}

void CPolygonDeferred::UnInitialize()
{

}

void CPolygonDeferred::Update()
{

}

void CPolygonDeferred::Draw(ID3D12GraphicsCommandList* cmdlist, ID3D12DescriptorHeap* Texture)
{
	HRESULT hr;

	//マトリクス設定
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	XMMATRIX world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

	//定数バッファ
	Constant* constant;
	hr = m_ConstantBuffer->Map(0, nullptr, (void**)&constant);
	assert(SUCCEEDED(hr));

	XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, XMMatrixTranspose(world * view * projection));
	constant->WVP = matrix;
	DirectX::XMStoreFloat4x4(&matrix, XMMatrixTranspose(world));
	constant->World = matrix;

	m_ConstantBuffer->Unmap(0, nullptr);

	cmdlist->SetGraphicsRootConstantBufferView(0, m_ConstantBuffer->GetGPUVirtualAddress());

	//頂点バッファ
	D3D12_VERTEX_BUFFER_VIEW vertexView;
	vertexView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(Vertex3D);
	vertexView.SizeInBytes = sizeof(Vertex3D) * 4;
	cmdlist->IASetVertexBuffers(0, 1, &vertexView);

	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { Texture };
	cmdlist->SetDescriptorHeaps(_countof(dh), dh);
	cmdlist->SetGraphicsRootDescriptorTable(1,
		Texture->GetGPUDescriptorHandleForHeapStart());


	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	
	//描画
	cmdlist->DrawInstanced(4, 1, 0, 0);
}
