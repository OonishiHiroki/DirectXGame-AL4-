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

void Model::CreateModel() {
	HRESULT result = S_FALSE;

	//ファイルストリーム
	std::ifstream file;
	// .objファイルを開く
	/*file.open("Resources/triangle2/triangle2.obj");*/
	const string modelname = "triangle_mat";
	const string filename = modelname + ".obj"; //triangle_mat.obj
	const string directoryPath = "Resources/" + modelname + "/"; //Resources/triangle_mat/
	file.open(directoryPath + filename); //Resources/triangle_mat/triangle_mat.obj

	// ファイルオープン失敗をチェック
	assert(!file.fail());
	vector<XMFLOAT3> positions; //頂点座標
	vector<XMFLOAT3> normals;   //法線ベクトル
	vector<XMFLOAT2> texcoords; //テクスチャUV
	//1行ずつ読み込み
	string line;
	while (getline(file, line)) {

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		//半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		//先頭文字列がmtllibならマテリアル
		if (key == "mtllib") {
			//マテリアルのファイル名読み込み
			string filename;
			line_stream >> filename;
			//マテリアル読み込み
			LoadMaterial(directoryPath, filename);
		}

		//先頭文字列がvなら頂点座標
		if (key == "v") {
			//X,Y,Z座標読み込み
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			//座標データに追加
			positions.emplace_back(position);
			//頂点データに追加
			/*VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);*/
		}

		//先頭文字列がvtならテクスチャ
		if (key == "vt") {
			//U,V成分読み込み
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			//V方向反転
			texcoord.y = 1.0f - texcoord.y;
			//座標データに追加
			texcoords.emplace_back(texcoord);

		}

		//先頭文字列がvnなら法線ベクトル
		if (key == "vn") {
			//X,Y,Z座標読み込み
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			//座標データに追加
			normals.emplace_back(normal);

		}

		//先頭文字列がfならポリゴン(三角形)
		if (key == "f") {
			//半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				//頂点インデックス1個分の文字列をストリームに変換して解析しやすくなる
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexTexcoord, indexNormal;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); //スラッシュを飛ばす
				index_stream >> indexNormal;
				//頂点データの追加
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				//インデックスデータの追加
				indices.emplace_back((unsigned short)indices.size());
			}
		}
	}

	//ファイルを閉じる
	file.close();

	std::vector<VertexPosNormalUv> realVertices;

	/*UINT sizeVB = static_cast<UINT>(sizeof(vertices));*/
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());
	/*UINT sizeIB = static_cast<UINT>(sizeof(indices));*/
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());


	// ヒーププロパティ
	CD3DX12_HEAP_PROPERTIES heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	// リソース設定
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeVB);

	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// 頂点バッファへのデータ転送
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		/*memcpy(vertMap, vertices, sizeof(vertices));*/
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビューの作成
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	/*vbView.SizeInBytes = sizeof(vertices);*/
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);


	// リソース設定
	resourceDesc.Width = sizeIB;

	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&heapProps, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&indexBuff));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {

		//// 全インデックスに対して
		//for (int i = 0; i < _countof(indices); i++)
		//{
		//	indexMap[i] = indices[i];	// インデックスをコピー
		//}
		std::copy(indices.begin(), indices.end(), indexMap);

		indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビューの作成
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	/*ibView.SizeInBytes = sizeof(indices);*/
	ibView.SizeInBytes = sizeIB;
}