#pragma once
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>


//class Model {
//private:
//	// DirectX::���ȗ�
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMMATRIX = DirectX::XMMATRIX;
//
//public:
//	// ���_�f�[�^�\����
//	struct VertexPosNormalUv {
//		XMFLOAT3 pos; // xyz���W
//		XMFLOAT3 normal; // �@���x�N�g��
//		XMFLOAT2 uv;  // uv���W
//	};
//
//private:
//	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//	// ���_�o�b�t�@
//	ComPtr<ID3D12Resource> vertBuff;
//	// �C���f�b�N�X�o�b�t�@
//	ComPtr<ID3D12Resource> indexBuff;
//	// ���_�o�b�t�@�r���[
//	D3D12_VERTEX_BUFFER_VIEW vbView;
//	// �C���f�b�N�X�o�b�t�@�r���[
//	D3D12_INDEX_BUFFER_VIEW ibView;
//	// ���_�f�[�^�z��
//	/*static VertexPosNormalUv vertices[vertexCount];*/
//	static std::vector<VertexPosNormalUv> vertices;
//	// ���_�C���f�b�N�X�z��
//	/*static unsigned short indices[planeCount * 3];*/
//	static std::vector<unsigned short> indices;
//};

