#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <string.h>



class Model {
private:
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	// 頂点データ構造体
	struct VertexPosNormalUv {
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// デバイス
	static ID3D12Device* device;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// インデックスバッファ
	static ComPtr<ID3D12Resource> indexBuff;
	// 頂点バッファビュー
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// インデックスバッファビュー
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// 頂点データ配列
	/*static VertexPosNormalUv vertices[vertexCount];*/
	static std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	/*static unsigned short indices[planeCount * 3];*/
	static std::vector<unsigned short> indices;

	/// <summary>
	/// モデル作成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// マテリアル
	/// </summary>
	static void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	//-----メンバ変数-----//

	//マテリアル
	struct Material {
		std::string name;				//マテリアル
		XMFLOAT3 ambient;				//アンビエント影響度
		XMFLOAT3 diffuse;				//ディフューズ影響度
		XMFLOAT3 specular;				//スペキュラー影響度
		float alpha;					//アルファ
		std::string textureFilename;	//テクスチャファイル名
		//コンストラクト
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

	//マテリアル
	static Material material;
};

