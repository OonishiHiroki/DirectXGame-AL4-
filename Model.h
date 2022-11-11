#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string.h>



class Model {
private:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// ���_�f�[�^�\����
	struct VertexPosNormalUv {
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// �f�o�C�X
	static ID3D12Device* device;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	static ComPtr<ID3D12Resource> indexBuff;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	/*static VertexPosNormalUv vertices[vertexCount];*/
	static std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	/*static unsigned short indices[planeCount * 3];*/
	static std::vector<unsigned short> indices;

	/// <summary>
	/// ���f���쐬
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// �}�e���A��
	/// </summary>
	static void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//-----�����o�ϐ�-----//

	//�}�e���A��
	struct Material {
		std::string name;				//�}�e���A��
		XMFLOAT3 ambient;				//�A���r�G���g�e���x
		XMFLOAT3 diffuse;				//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;				//�X�y�L�����[�e���x
		float alpha;					//�A���t�@
		std::string textureFilename;	//�e�N�X�`���t�@�C����
		//�R���X�g���N�g
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//�}�e���A��
	static Material material;
};

