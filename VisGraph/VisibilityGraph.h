#pragma once

#include "typeOperation.h"
// #include "MatrixData.h"
// #include "cudaStreamlineRenderer.h"
// #include "cudaDeviceMem.h"
// #include "cudaStreamlineTracer.h"
// #include "definition.h"
#include "VolumeData.h"
// #include "RandomSample.h"
// #include "StreamlineResample.h"
// #include "cnpy.h"
// #include "WindowsTimer.h"
// #include "dbscan.h"
#include <vector>
// #include <rapidjson/document.h>
// #include <thrust/sort.h>
// #include <thrust/host_vector.h>
// #include <thrust/execution_policy.h>
// #include "Registration.h"
#include <numeric>
//#include"FlowEncoder.h"
using namespace std;

#define TIMING_RESULT
#ifdef TIMING_RESULT
#define PRINT_TIME printf
#define START_TIMER mTimer.start()
#endif

#define FRECHET_NOT_INIT	-1.0f
#define FRECHET_INIT_THRESH	-0.9f
#define FRECHET_INFINITY	1e30



// typedef struct {
// 	std::string directory;
// 	std::string vec_field_path;
// 	std::string vec_hdr_path;
// 	std::string streamline_path;
// 	std::string vis_graph_path;
// 	std::string binormal_curve_path;
// 	std::string binormal_graph_path;
// 	std::string normal_curve_path;
// 	std::string normal_graph_path;
// 	std::string feature_path;
// 	std::string sample_vg_path;
// 	std::string all_vg_path;
// 	std::string latent_feature_path;
// 	std::string latent_segment_map_path;
// 	std::string point_latent_map_path;
// 	std::string tsne_layout_path;
// 	std::string tsne_latent_path;
// 	std::string tsne_visgraph_path;
// 	std::string tsne_id_path;
// 	std::string curvature_path;
// 	std::string vis_kunhua_dis_path;
// 	std::string vis_kunhua_dis_path2;
// 	std::string vis_kunhua_dis_file;
// 	std::string vis_kunhua_dis_file_path;
// 	std::vector<float> sample_steps;
// 	std::vector<float> filter_scales;
// 	std::vector<float> resample_scales;
// 	float segment_length;
// 	float radius;
// 	int store_gap_streamline;
// 	int store_gap_normal_binormal;
// 	int local_sample_num;
// 	int dist_thresh_num;
// 	int max_streamline_length;
// 	int num_streamlines;
// 	int num_random_samples;
// 	float normalize_factor;
// 	float reverse_normalize_factor;
// 	int latent_feature_dim;
// 	int latent_feature_full_dim;
// 	int sample_steps_num;
// 	int filter_scales_num;
// 	int resample_scales_num;
// 	int latent_feature_num;
// 	float curvature_sample_rate;
// } VisGraphMetaInfo;


class VisGraph {
public:
	VisGraph(const int& _n, const int& offset, float* adj_mat_data,
		float* degree_mat_data, const int& num_dist_thresh, float* latent_mat_data, const int& latent_dim) :
		n(_n),
		matrix_offset(offset),
		adj_mat(_n, _n, adj_mat_data),
		degree_mat(num_dist_thresh, _n, degree_mat_data),
		latent_mat(latent_dim, _n, latent_mat_data)
	{
	}

	~VisGraph() {}

};

class VisGraphDataManager {
public:
	VisGraphDataManager(const char* directory_path, const bool& init_dbscan = true)//dir_path = "D:/data/project_data/VisibilityGraph/"
	{
		readMetaInfo(directory_path);

		//set default colors
		//����Ĭ�ϵ���ɫֵӳ��
		mContextColor = makeVec4f(ColorMap::getColorByName(ColorMap::Lavender_gray).xyz, 0.008f);
		mMatchColor = makeVec4f(ColorMap::getColorByName(ColorMap::Brandeis_blue).xyz, 0.2f);
		mQueryColor = makeVec4f(ColorMap::getColorByName(ColorMap::Harvard_crimson).xyz, 1.0f);

		////read or produce streamline pool
		//if (!readStreamlinePool(mPool, meta.streamline_path.c_str())) {
		//	genAndSaveRandomPool();
		//}

		//�������ݼ�
		std::string data_path = "D:/data/flow/";

		dataset = "brain";
		std::string vec_hdr_path = data_path + dataset + ".hdr";
		//std::string streamline_path = data_path + dataset + ".stl";
		std::string streamline_path = "D:/data/brain/003_S_4081_1_fibers_FA_normed2.stl";
		//������߳� mpool
		mDim = read_header(vec_hdr_path.c_str());
		readStreamlinePool(mPool, streamline_path.c_str(), dataset == "brain");

		//allocate data: this need to be performed before computing the distance matrices
		//allocateData();

		//for render data
		//StreamlinePool renderPool(mPool);

		//create renderer: should be called after reading the streamline pool
		//mRenderer = new cudaStreamlineRenderer(mPool.streamlines.data(), mPool.points.data(), mPool.streamlines.size(), 8, meta.radius);

// 		//�Ⱦ������
// 		resample(mPool, 0.3);
// 		//���Ȳ���sample_numpoint����
// 		getEqualPointStreamlinePool(mPool, 32);
// 		//��׼��
// 		normalization(mPool);

		printf("Dataset is: %s\n", dataset.c_str());
		printf("Dataset dim is: %d X %d X %d ,segment length is:%2.1f \n including %d points and %d streamlines, sample points number is: %d . \n\n", mDim.x, mDim.y, mDim.z, meta.segment_length, getNumPoints(), getNumStreamlines(), getNumPoints() / getNumStreamlines());

		//���㲢����3��representations
		//computeAndSaveVisiblityGraphs();
		//computeAndSaveRotate(mPool);
		//computeAndSaveLocation(mPool);

		//���㵱ǰ���ݼ�������󣬲��洢����Ӧλ��
		//computeAndSaveDistanceMatrix(mPool);

		//��ȡ������󣬷ŵ�GraphDisplay�У�����MatrixData��Ȼ��setData
		//readAndSetDisData();

	}

	~VisGraphDataManager() {
	}

	//����洢�ɼ�ͼ
	void computeAndSaveVisiblityGraphs() {
		START_TIMER;
		PRINT_TIME("Timing: Computing all Visibility Graphs.");

		std::vector<VisGraphCompTask> tasks;
		tasks.reserve((mDistData.size() - mPool.points.size()) / 2);
		VisGraphCompTask t;
		//create tasks
		//��������
		int data_offset, n;
		for (int i = 0; i < mPool.streamlines.size(); ++i) {
			const Streamline& s = mPool.streamlines[i];
			n = s.numPoint;
			data_offset = getVisGraphOfStreamline(i)->matrix_offset;
			for (int j = 0; j < n; ++j) {
				//t.uidΪ�����ϵĵ�j����
				t.uid = s.start + j;
				for (int k = j + 1; k < n; ++k) {
					//t.vidΪ�����ϴ�start��ʼ��k���㣨��j����֮��
					t.vid = s.start + k;
					//t.dist_locΪÿ�����ߵ�distance matrix�ĵ�j�е�k�У������λ��(j,k)
					t.dist_loc = data_offset + j * n + k;
					tasks.push_back(t);
				}
			}
		}
		//compute the first half of the matrix visibility graph
		//����������distance matrix
		computeVisibiltyGraphs_h(mDistData.data(), tasks.data(), mRenderer->getPoints_d(), mDistData.size(), 16, tasks.size());

		//fill the other half of the matricies
		//������ͨ�������ǶԳƵõ�
		for (int i = 0; i < mGraphs.size(); ++i) {
			float** dist_mat = mGraphs[i]->adj_mat.getMatrixPointer();
			int n = mGraphs[i]->n;
			for (int j = 0; j < n; ++j) {
				//�Խ���
				dist_mat[j][j] = 0.0f;
				//������
				for (int k = 0; k < j; ++k) {
					dist_mat[j][k] = dist_mat[k][j];
				}
			}
			//��ÿ�����ߵ�distance matrix������ʼ��ͽ������id
			const Streamline& s = mPool.streamlines[i];
			dist_mat[0][0] = s.start;
			//dist_mat[mGraphs.size() - 1][mGraphs.size() - 1] = s.start + s.numPoint - 1;
		}

		std::vector<float> vg;
		std::vector<vector<float>> vgs;
		for (int i = 0; i < mPool.streamlines.size(); ++i) {
			Streamline currentline = mPool.streamlines[i];
			for (int m = currentline.start; m < currentline.numPoint; ++m) {
				for (int n = currentline.start; n < currentline.numPoint; ++n) {
					float dist = dist3d(mPool.points[m], mPool.points[n]);
					vg.push_back(dist);
				}
			}
			vgs.push_back(vg);
		}

		std::vector<float> vgs_data;
		for (int i = 0; i < vgs.size(); ++i) {
			for (int j = 0; j < vgs[i].size(); ++j) {
				vgs_data.push_back(vgs[i][j]);
			}
		}

		PRINT_TIME(" Finish in %5.3f.\n\n", mTimer.end());

		std::string vis_graph_path = file_path + dataset + "/visibilitygraph.dat";
		write_array(vgs_data.data(), vgs_data.size(), vis_graph_path.c_str());
	}

	VisGraphMetaInfo meta;

	std::string file_path = "D:/data/project_data/VisibilityGraph/result/";

	std::vector<std::string> datasets = { "abc", "bernard", "brain", "computer_room","crayfish", "cylinder","electro3D","vessel",
			"plume","random-5cp","two_swirl","tornado" };

	std::string dataset = "brain";
	vec3i mDim;
	StreamlinePool mPool;

	std::vector<std::vector<float>> mKunhuaDisMats;
	std::vector<float> mDistData;
	std::vector<float> mKunHuaDisMat;
	std::vector<float> mKunHuaDisMat_row;

	std::vector<float> mCurvatures;
	cudaStreamlineRenderer* mRenderer;

	std::vector<float> mDistThreshes;
	std::vector<float> mDegreeData;
	std::vector<float> mDegreeAvg;
	std::vector<float> mDegreeVar;
	std::vector<float> mLatentData;
	Range mLatentRange;
	std::vector<StreamlineSegment> mLatentSegmentMap;
	std::vector<int> mLatentFeatureLevels;
	std::vector<int> mPointLatentLookup;
	std::vector<float> mLatentDisplayData;
	std::vector<vec2f> mTsneLayout;
	std::vector<float> mTsneVisGraphData;
	std::vector<int> mLatentTsneMap;
	std::vector<std::vector<int>> mTsneLatentMap;
	std::vector<int> mTsneColorIds;
	std::vector<int> mTsneMatchIds;
	std::vector<int> mTsneQueryIds;


	std::vector<float> mKunHuaDisMat_row2;

	std::vector<vec3f> mNormalCurves;
	std::vector<float> mNormalDistData;
	std::vector<vec3f> mBinormalCurves;
	std::vector<float> mBinormalDistData;
	std::vector<VisGraph*> mGraphs;

	std::vector<vec4f> TandS;

	vec4f mQueryColor;
	vec4f mMatchColor;
	vec4f mContextColor;
};
