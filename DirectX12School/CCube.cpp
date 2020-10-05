#include "CCube.h"
#include "main.h"
#include "renderer.h"


void CCube::Initialized()
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


	resourceDesc.Width = sizeof(Vertex3D) * 24;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_VertexBuffer));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	//インデックスバッファの作成
	resourceDesc.Width = sizeof(Vertex3D) * 36;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_IndexBuffer));


	resourceDesc.Width = 256;
	hr = device->CreateCommittedResource(&heapPropertes, D3D12_HEAP_FLAG_NONE,
		&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr
		, IID_PPV_ARGS(&m_ConstantBuffer));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	//���_�f�[�^�̏�������
	Vertex3D* vertex{};
	hr = m_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&vertex));
	//ThrowIfFailed(hr);
	//assert(SUCCEEDED(hr));

	vertex[0].Position = { -1.0f, 1.0f,  1.0f };
	vertex[1].Position = { 1.0f, 1.0f,  1.0f };
	vertex[2].Position = { -1.0f, 1.0f, -1.0f };
	vertex[3].Position = { 1.0f, 1.0f, -1.0f };
	vertex[0].Normal = { 0.0,1.0,0.0 };
	vertex[1].Normal = { 0.0,1.0,0.0 };
	vertex[2].Normal = { 0.0,1.0,0.0 };
	vertex[3].Normal = { 0.0,1.0,0.0 };
	vertex[0].TexCoord = { 0.0,0.0 };
	vertex[1].TexCoord = { 0.0,1.0 };
	vertex[2].TexCoord = { 1.0,0.0 };
	vertex[3].TexCoord = { 1.0,1.0 };

	vertex[4].Position = { 1.0f, -1.0f,  1.0f };
	vertex[5].Position = { -1.0f, -1.0f,  1.0f };
	vertex[6].Position = { 1.0f, -1.0f, -1.0f };
	vertex[7].Position = { -1.0f, -1.0f, -1.0f };
	vertex[4].Normal = { 0.0,-1.0,0.0 };
	vertex[5].Normal = { 0.0,-1.0,0.0 };
	vertex[6].Normal = { 0.0,-1.0,0.0 };
	vertex[7].Normal = { 0.0,-1.0,0.0 };
	vertex[4].TexCoord = { 0.0,0.0 };
	vertex[5].TexCoord = { 0.0,1.0 };
	vertex[6].TexCoord = { 1.0,0.0 };
	vertex[7].TexCoord = { 1.0,1.0 };

	vertex[8].Position = { 1.0f, 1.0f,  -1.0f };
	vertex[9].Position = { 1.0f, 1.0f,  1.0f };
	vertex[10].Position = { 1.0f, -1.0f, -1.0f };
	vertex[11].Position = { 1.0f, -1.0f, 1.0f };
	vertex[8].Normal =  { 1.0,0.0,0.0 };
	vertex[9].Normal =  { 1.0,0.0,0.0 };
	vertex[10].Normal = { 1.0,0.0,0.0 };
	vertex[11].Normal = { 1.0,0.0,0.0 };
	vertex[8].TexCoord = { 0.0,0.0 };
	vertex[9].TexCoord = { 0.0,1.0 };
	vertex[10].TexCoord = { 1.0,0.0 };
	vertex[11].TexCoord = { 1.0,1.0 };


	vertex[12].Position = { -1.0f, 1.0f,  1.0f };
	vertex[13].Position = { -1.0f, 1.0f,  -1.0f };
	vertex[14].Position = { -1.0f, -1.0f, 1.0f };
	vertex[15].Position = { -1.0f, -1.0f, -1.0f };
	vertex[12].Normal = { -1.0,0.0,0.0 };
	vertex[13].Normal = { -1.0,0.0,0.0 };
	vertex[14].Normal = { -1.0,0.0,0.0 };
	vertex[15].Normal = { -1.0,0.0,0.0 };
	vertex[12].TexCoord = { 0.0,0.0 };
	vertex[13].TexCoord = { 0.0,1.0 };
	vertex[14].TexCoord = { 1.0,0.0 };
	vertex[15].TexCoord = { 1.0,1.0 };


	vertex[16].Position = { -1.0f, 1.0f, -1.0f };
	vertex[17].Position = { 1.0f, 1.0f,  -1.0f };
	vertex[18].Position = { -1.0f, -1.0f, -1.0f };
	vertex[19].Position = { 1.0f, -1.0f, -1.0f };
	vertex[16].Normal = { 0.0,0.0,-1.0 };
	vertex[17].Normal = { 0.0,0.0,-1.0 };
	vertex[18].Normal = { 0.0,0.0,-1.0 };
	vertex[19].Normal = { 0.0,0.0,-1.0 };
	vertex[16].TexCoord = { 0.0,0.0 };
	vertex[17].TexCoord = { 0.0,1.0 };
	vertex[18].TexCoord = { 1.0,0.0 };
	vertex[19].TexCoord = { 1.0,1.0 };


	vertex[20].Position = { 1.0f, 1.0f,  1.0f };
	vertex[21].Position = { -1.0f, 1.0f,  1.0f };
	vertex[22].Position = { 1.0f, -1.0f, 1.0f };
	vertex[23].Position = { -1.0f, -1.0f, 1.0f };
	vertex[20].Normal = { 0.0,0.0,1.0 };
	vertex[21].Normal = { 0.0,0.0,1.0 };
	vertex[22].Normal = { 0.0,0.0,1.0 };
	vertex[23].Normal = { 0.0,0.0,1.0 };
	vertex[20].TexCoord = { 0.0,0.0 };
	vertex[21].TexCoord = { 0.0,1.0 };
	vertex[22].TexCoord = { 1.0,0.0 };
	vertex[23].TexCoord = { 1.0,1.0 };



	m_VertexBuffer->Unmap(0, nullptr);

	//インデックスデータの書き込み
	unsigned short* index;
	hr = m_IndexBuffer->Map(0, nullptr, (void**)&index);
	ThrowIfFailed(hr);

	for (unsigned short i = 0; i < 6; i++)
	{
		index[i * 6 + 0] = i * 4 + 0;
		index[i * 6 + 1] = i * 4 + 1;
		index[i * 6 + 2] = i * 4 + 2;

		index[i * 6 + 3] = i * 4 + 1;
		index[i * 6 + 4] = i * 4 + 3;
		index[i * 6 + 5] = i * 4 + 2;
	}

	m_texture = std::make_unique<CTexture>();
	m_texture->load("data/field004.tga");

	m_Rotation = { 0.0f,0.0f,0.0f };
}

void CCube::Uninitialize()
{

}

void CCube::Update()
{
	m_Rotation.x += 0.01f;
	m_Rotation.y += 0.01f;
}

void CCube::Draw(ID3D12GraphicsCommandList* cmdlist)
{
	HRESULT hr;

	//マトリクス設定
	XMMATRIX view = XMMatrixLookAtLH({ 0.0f,0.0f,-5.0f }, { 0.0, 0.0, 0.0 }, { 0.0f,1.0,0.0f });
	XMMATRIX projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 500.0f);
	XMMATRIX world = XMMatrixIdentity();

	world *= DirectX::XMMatrixRotationRollPitchYaw(m_Rotation.x, m_Rotation.y, m_Rotation.z);
	world *= XMMatrixTranslation(0.0f, 0.0f, 0.0f);

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
	D3D12_VERTEX_BUFFER_VIEW vertexView{};
	vertexView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	vertexView.StrideInBytes = sizeof(Vertex3D);
	vertexView.SizeInBytes = sizeof(Vertex3D) * 24;
	cmdlist->IASetVertexBuffers(0, 1, &vertexView);

	//インデックスバッファの設定
	D3D12_INDEX_BUFFER_VIEW indexView{};
	indexView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	indexView.SizeInBytes = sizeof(unsigned short)*36; //uint_least16_t
	indexView.Format = DXGI_FORMAT_R16_UINT;
	cmdlist->IASetIndexBuffer(&indexView);


	//テクスチャ設定
	ID3D12DescriptorHeap* dh[] = { *m_texture->GetSDescriptorHeap().GetAddressOf() };
	cmdlist->SetDescriptorHeaps(_countof(dh), dh);
	cmdlist->SetGraphicsRootDescriptorTable(1, m_texture->GetSDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());


	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//描画
	cmdlist->DrawIndexedInstanced(36, 1, 0, 0,0);
}
