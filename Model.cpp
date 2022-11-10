#include "Model.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

#pragma comment(lib, "d3dcompiler.lib")

D3D12_VERTEX_BUFFER_VIEW Model::vbView{};
D3D12_INDEX_BUFFER_VIEW Model::ibView{};

ComPtr<ID3D12Resource> Model::vertBuff;
ComPtr<ID3D12Resource> Model::indexBuff;

ID3D12Device* Model::device = nullptr;
std::vector<Model::VertexPosNormalUv> Model::vertices;
std::vector<unsigned short> Model::indices;

Model::Material Model::material;

//void Model::CreateModel() {
//	HRESULT result = S_FALSE;
//
//	//�t�@�C���X�g���[��
//	std::ifstream file;
//	// .obj�t�@�C�����J��
//	/*file.open("Resources/triangle2/triangle2.obj");*/
//	const string modelname = "triangle_mat";
//	const string filename = modelname + ".obj"; //triangle_mat.obj
//	const string directoryPath = "Resources/" + modelname + "/"; //Resources/triangle_mat/
//	file.open(directoryPath + filename); //Resources/triangle_mat/triangle_mat.obj
//
//	// �t�@�C���I�[�v�����s���`�F�b�N
//	assert(!file.fail());
//	vector<XMFLOAT3> positions; //���_���W
//	vector<XMFLOAT3> normals;   //�@���x�N�g��
//	vector<XMFLOAT2> texcoords; //�e�N�X�`��UV
//	//1�s���ǂݍ���
//	string line;
//	while (getline(file, line)) {
//
//		//1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
//		std::istringstream line_stream(line);
//
//		//���p�X�y�[�X��؂�ōs�̐擪��������擾
//		string key;
//		getline(line_stream, key, ' ');
//
//		//�擪������mtllib�Ȃ�}�e���A��
//		if (key == "mtllib") {
//			//�}�e���A���̃t�@�C�����ǂݍ���
//			string filename;
//			line_stream >> filename;
//			//�}�e���A���ǂݍ���
//			LoadMaterial(directoryPath, filename);
//		}
//
//		//�擪������v�Ȃ璸�_���W
//		if (key == "v") {
//			//X,Y,Z���W�ǂݍ���
//			XMFLOAT3 position{};
//			line_stream >> position.x;
//			line_stream >> position.y;
//			line_stream >> position.z;
//			//���W�f�[�^�ɒǉ�
//			positions.emplace_back(position);
//			//���_�f�[�^�ɒǉ�
//			/*VertexPosNormalUv vertex{};
//			vertex.pos = position;
//			vertices.emplace_back(vertex);*/
//		}
//
//		//�擪������vt�Ȃ�e�N�X�`��
//		if (key == "vt") {
//			//U,V�����ǂݍ���
//			XMFLOAT2 texcoord{};
//			line_stream >> texcoord.x;
//			line_stream >> texcoord.y;
//			//V�������]
//			texcoord.y = 1.0f - texcoord.y;
//			//���W�f�[�^�ɒǉ�
//			texcoords.emplace_back(texcoord);
//
//		}
//
//		//�擪������vn�Ȃ�@���x�N�g��
//		if (key == "vn") {
//			//X,Y,Z���W�ǂݍ���
//			XMFLOAT3 normal{};
//			line_stream >> normal.x;
//			line_stream >> normal.y;
//			line_stream >> normal.z;
//			//���W�f�[�^�ɒǉ�
//			normals.emplace_back(normal);
//
//		}
//
//		//�擪������f�Ȃ�|���S��(�O�p�`)
//		if (key == "f") {
//			//���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
//			string index_string;
//			while (getline(line_stream, index_string, ' ')) {
//				//���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷���Ȃ�
//				std::istringstream index_stream(index_string);
//				unsigned short indexPosition, indexTexcoord, indexNormal;
//				index_stream >> indexPosition;
//				index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
//				index_stream >> indexTexcoord;
//				index_stream.seekg(1, ios_base::cur); //�X���b�V�����΂�
//				index_stream >> indexNormal;
//				//���_�f�[�^�̒ǉ�
//				VertexPosNormalUv vertex{};
//				vertex.pos = positions[indexPosition - 1];
//				vertex.normal = normals[indexNormal - 1];
//				vertex.uv = texcoords[indexTexcoord - 1];
//				vertices.emplace_back(vertex);
//				//�C���f�b�N�X�f�[�^�̒ǉ�
//				indices.emplace_back((unsigned short)indices.size());
//			}
//		}
//	}
//
//	//�t�@�C�������
//	file.close();
//
//	std::vector<VertexPosNormalUv> realVertices;
//
//	/*UINT sizeVB = static_cast<UINT>(sizeof(vertices));*/
//	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
//	/*UINT sizeIB = static_cast<UINT>(sizeof(indices));*/
//	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
//
//
//	// �q�[�v�v���p�e�B
//	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
//	// ���\�[�X�ݒ�
//	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);
//
//	// ���_�o�b�t�@����
//	result = device->CreateCommittedResource(
//		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	// ���_�o�b�t�@�ւ̃f�[�^�]��
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	if (SUCCEEDED(result)) {
//		/*memcpy(vertMap, vertices, sizeof(vertices));*/
//		std::copy(vertices.begin(), vertices.end(), vertMap);
//		vertBuff->Unmap(0, nullptr);
//	}
//
//	// ���_�o�b�t�@�r���[�̍쐬
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	/*vbView.SizeInBytes = sizeof(vertices);*/
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(vertices[0]);
//
//
//	// ���\�[�X�ݒ�
//	resourceDesc.Width = sizeIB;
//
//	// �C���f�b�N�X�o�b�t�@����
//	result = device->CreateCommittedResource(
//		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&indexBuff));
//
//	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
//	unsigned short* indexMap = nullptr;
//	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
//	if (SUCCEEDED(result)) {
//
//		//// �S�C���f�b�N�X�ɑ΂���
//		//for (int i = 0; i < _countof(indices); i++)
//		//{
//		//	indexMap[i] = indices[i];	// �C���f�b�N�X���R�s�[
//		//}
//		std::copy(indices.begin(), indices.end(), indexMap);
//
//		indexBuff->Unmap(0, nullptr);
//	}
//
//	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
//	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
//	ibView.Format = DXGI_FORMAT_R16_UINT;
//	/*ibView.SizeInBytes = sizeof(indices);*/
//	ibView.SizeInBytes = sizeIB;
//}