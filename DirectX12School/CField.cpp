#include "CField.h"
#include "main.h"
#include "renderer.h"


void CField::Initialized()
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


	resourceDesc.Width = sizeof(Vertex3D) * 4;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_VertexBuffer));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	resourceDesc.Width = 256;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_ConstantBuffer));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	//���_�f�[�^�̏�������
	Vertex3D* buffer{};
	hr = m_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&buffer));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	buffer[0].Position = { -10.0f, 0.0f,  10.0f };
	buffer[1].Position = {  10.0f, 0.0f,  10.0f };
	buffer[2].Position = { -10.0f, 0.0f, -10.0 };
	buffer[3].Position = {  10.0f, 0.0f, -10.0 };
	buffer[0].Normal = { 0.0,1.0,0.0 };
	buffer[1].Normal = { 0.0,1.0,0.0 };
	buffer[2].Normal = { 0.0,1.0,0.0 };
	buffer[3].Normal = { 0.0,1.0,0.0 };
	buffer[0].TexCoord = { 0.0,0.0 };
	buffer[1].TexCoord = { 0.0,10.0 };
	buffer[2].TexCoord = { 10.0,0.0 };
	buffer[3].TexCoord = { 10.0,10.0 };

	m_VertexBuffer->Unmap(0, nullptr);

	m_texture = std::make_unique<CTexture>();
	m_texture->load("data/field004.tga");
}

void CField::Uninitialize()
{

}

void CField::Update()
{

}

void CField::Draw(ID3D12GraphicsCommandList* cmdlist)
{
	HRESULT hr;

	//マトリクス設定
	XMMATRIX view = XMMatrixLookAtLH({ 0.0f,0.0f,-5.0f }, { 0.0, 0.0, 0.0 }, {0.0f,1.0,0.0f});
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH/SCREEN_HEIGHT,1.0f, 500.0f);
	XMMATRIX world = XMMatrixTranslation(0.0f, -2.0f, 0.0f);

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

	//定数バッファの作成
	

	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { *m_texture->GetSDescriptorHeap().GetAddressOf() };
	cmdlist->SetDescriptorHeaps(_countof(dh), dh);
	cmdlist->SetGraphicsRootDescriptorTable(1, m_texture->GetSDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());


	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//描画
	cmdlist->DrawInstanced(4, 1, 0, 0);
}
