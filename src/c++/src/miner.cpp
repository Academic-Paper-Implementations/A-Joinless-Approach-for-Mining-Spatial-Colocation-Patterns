#include "miner.h"
#include "utils.h"
#include "neighborhood_mgr.h"
#include "types.h"
#include <algorithm>
#include <set>
#include <string>
#include <omp.h> 
#include <iomanip>
#include <chrono>
#include <iostream>

std::vector<Colocation> JoinlessMiner::mineColocations(
    double minPrev, 
    NeighborhoodMgr* neighborhoodMgr, 
    const std::vector<SpatialInstance>& instances,
    ProgressCallback progressCb
) {
	// Start timer
    auto minerStart = std::chrono::high_resolution_clock::now();

    // Assign parameters to member variables for use in other methods
    this->progressCallback = progressCb;
    
    int k = 2;
    std::vector<FeatureType> types = getAllObjectTypes(instances);
    std::map<FeatureType, int> featureCount = countInstancesByFeature(instances);
    std::vector<Colocation> prevColocations;
    std::vector<ColocationInstance> cliqueInstances;
    std::vector<ColocationInstance> prevCliqueInstances;
    std::vector<Colocation> allPrevalentColocations;

    // Estimate total iterations (max pattern size is number of types)
    int maxK = types.size();
    int currentIteration = 0;
    int totalIterations = 0; // Will be updated as we go

    if (progressCallback) {
        progressCallback(0, maxK, "Initializing mining process...", 0.0);
    }

    for (auto t : types) prevColocations.push_back({t});

    while (!prevColocations.empty()) {
        currentIteration++;
        totalIterations = currentIteration;
        
        // Calculate progress: use a conservative estimate that we might go up to maxK
        // But cap at 95% until we're actually done
        double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
        
        if (progressCallback) {
            progressCallback(currentIteration, maxK, 
                "Processing k=" + std::to_string(k) + " patterns...", 
                progressPercent);
        }
        std::vector<ColocationInstance> starInstances;

		// 1. Generate candidate patterns of size k
        auto t1_start = std::chrono::high_resolution_clock::now();
        std::vector<Colocation> candidates = generateCandidates(prevColocations);
        auto t1_end = std::chrono::high_resolution_clock::now();
        printDuration("generateCandidates (k=" + std::to_string(k) + ")", t1_start, t1_end);

        if (candidates.empty()) {
            if (progressCallback) {
                progressCallback(currentIteration, maxK, 
                    "No more candidates found. Mining completed.", 
                    100.0);
            }
            break;
        }
        
        if (progressCallback) {
            double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
            progressCallback(currentIteration, maxK, 
                "Filtering star instances for " + std::to_string(candidates.size()) + " candidates...", 
                progressPercent);
        }
        
		// 2. Filter star instances for each candidate
        auto t2_start = std::chrono::high_resolution_clock::now();
        for (auto t : types) {
            for (const auto& starNeigh : neighborhoodMgr->getAllStarNeighborhoods()) {
                if (starNeigh.first == t) {
                    std::vector<ColocationInstance> found = filterStarInstances(candidates, starNeigh);
                    starInstances.insert(starInstances.end(), found.begin(), found.end());
                }
            }
        }
        auto t2_end = std::chrono::high_resolution_clock::now();
        printDuration("filterStarInstances (Total) (k=" + std::to_string(k) + ")", t2_start, t2_end);

        if (k==2){
            cliqueInstances = starInstances;
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Found " + std::to_string(starInstances.size()) + " star instances (k=2)...", 
                    progressPercent);
            }
        }else{
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Selecting prevalent colocations (coarse filter)...", 
                    progressPercent);
            }

			// 3. Select prevalent colocations using coarse filter
            auto t3_start = std::chrono::high_resolution_clock::now();
            candidates = selectPrevColocations(candidates, starInstances, minPrev, featureCount);
            auto t3_end = std::chrono::high_resolution_clock::now();
            printDuration("selectPrevColocations (Coarse) (k=" + std::to_string(k) + ")", t3_start, t3_end);
            
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "Filtering clique instances...", 
                    progressPercent);
            }

			// 4. Filter clique instances for candidates
            auto t4_start = std::chrono::high_resolution_clock::now();
            cliqueInstances = filterCliqueInstances(
                candidates,
                starInstances,
                prevCliqueInstances
            );
            auto t4_end = std::chrono::high_resolution_clock::now();
            printDuration("filterCliqueInstances (k=" + std::to_string(k) + ")", t4_start, t4_end);
        }
        
        if (progressCallback) {
            double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
            progressCallback(currentIteration, maxK, 
                "Selecting final prevalent colocations...", 
                progressPercent);
        }

        auto t5_start = std::chrono::high_resolution_clock::now();
        prevColocations = selectPrevColocations(
            candidates,
            cliqueInstances,
            minPrev,
            featureCount
        );
        auto t5_end = std::chrono::high_resolution_clock::now();
        printDuration("selectPrevColocations (Final) (k=" + std::to_string(k) + ")", t5_start, t5_end);
        

        if (!prevColocations.empty()) {
             allPrevalentColocations.insert(
                 allPrevalentColocations.end(), 
                 prevColocations.begin(), 
                 prevColocations.end()
             );

             std::cout << "[DEBUG] Found " << prevColocations.size() << " prevalent patterns. Details:\n";
             
             if (progressCallback) {
                 double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                 progressCallback(currentIteration, maxK, 
                     "Found " + std::to_string(prevColocations.size()) + " prevalent k=" + std::to_string(k) + " colocations", 
                     progressPercent);
             }
        } else {
            if (progressCallback) {
                double progressPercent = std::min(95.0, (static_cast<double>(currentIteration) / maxK) * 95.0);
                progressCallback(currentIteration, maxK, 
                    "No prevalent k=" + std::to_string(k) + " colocations found", 
                    progressPercent);
            }
        }
        
        prevCliqueInstances = std::move(cliqueInstances);

        k++;
    }
    
    if (progressCallback) {
        progressCallback(maxK, maxK, 
            "Mining completed! Total prevalent colocations: " + std::to_string(allPrevalentColocations.size()), 
            100.0);
    }

    auto minerEnd = std::chrono::high_resolution_clock::now();
    printDuration("TOTAL MINING TIME", minerStart, minerEnd);
    
    return allPrevalentColocations;
}


std::vector<Colocation> JoinlessMiner::generateCandidates(
    const std::vector<Colocation>& prevPrevalent) 
{
    std::vector<Colocation> candidates;
    
    if (prevPrevalent.empty()) {
        return candidates;
    }
    
    size_t patternSize = prevPrevalent[0].size();
    
    std::set<Colocation> prevSet(prevPrevalent.begin(), prevPrevalent.end());
    
    // Generate candidate
    for (size_t i = 0; i < prevPrevalent.size(); i++) {
        for (size_t j = i + 1; j < prevPrevalent.size(); j++) {
            // Take prefix of k-1 first element
            Colocation prefix1(prevPrevalent[i].begin(), 
                             prevPrevalent[i].end() - 1);
            Colocation prefix2(prevPrevalent[j].begin(), 
                             prevPrevalent[j].end() - 1);
            
            // Just join when the prefix is equal
            if (prefix1 != prefix2) {
                continue;
            }
            
            // Generate new candidate
            std::set<FeatureType> candidateSet(prevPrevalent[i].begin(), 
                                              prevPrevalent[i].end());
            candidateSet.insert(prevPrevalent[j].back());
            
            if (candidateSet.size() != patternSize + 1) {
                continue;
            }
            
            Colocation candidate(candidateSet.begin(), candidateSet.end());
            
            // APRIORI PRUNING
            bool allSubsetsValid = true;
            std::vector<FeatureType> candFeatures = candidate;
            
            for (size_t idx = 0; idx < candFeatures.size(); idx++) {
                Colocation subset = candFeatures; 
                subset.erase(subset.begin() + idx);
                
                if (prevSet.find(subset) == prevSet.end()) {
                    allSubsetsValid = false;
                    break;
                }
            }
            
            if (allSubsetsValid) {
                candidates.push_back(candidate);
            }
        }
    }
    
    // Remove duplicate
    std::sort(candidates.begin(), candidates.end());
    candidates.erase(std::unique(candidates.begin(), candidates.end()), 
                     candidates.end());

    return candidates;
}


std::vector<ColocationInstance> JoinlessMiner::filterStarInstances(
    const std::vector<Colocation>& candidates,
    const std::pair<FeatureType, std::vector<StarNeighborhood>>& starNeigh)
{
    std::vector<ColocationInstance> filteredInstances;
    FeatureType centerType = starNeigh.first;

    // Filter candidates to only those with this center type as first element
    std::vector<const Colocation*> relevantCandidates;
    for (const auto& cand : candidates) {
        if (!cand.empty() && cand[0] == centerType) {
            relevantCandidates.push_back(&cand);
        }
    }

    if (relevantCandidates.empty()) return filteredInstances;

    // Iterate through each star neighborhood
    for (const auto& star : starNeigh.second) {

        // Build a map of neighbors by feature type for fast lookup
        // Using const pointer since star is const
        std::unordered_map<FeatureType, std::vector<const SpatialInstance*>> neighborMap;

        for (auto neighbor : star.neighbors) {
            neighborMap[neighbor->type].push_back(neighbor);
        }

        // Check each relevant candidate pattern
        for (const auto* candPtr : relevantCandidates) {
            const auto& candidate = *candPtr;

            std::vector<const SpatialInstance*> currentInstance;
            currentInstance.reserve(candidate.size());

            // Add center instance as first element
            currentInstance.push_back(star.center);

            // Recursive function to find combinations
            findCombinations(candidate, 1, currentInstance, neighborMap, filteredInstances);
        }
    }

    return filteredInstances;
}


std::vector<ColocationInstance> JoinlessMiner::filterCliqueInstances(
    const std::vector<Colocation>& candidates,
    const std::vector<ColocationInstance>& instances,
    const std::vector<ColocationInstance>& prevInstances
) {
    // 1. TỐI ƯU HÓA TRA CỨU (PRE-CALCULATION)
    // Thay vì duyệt prevInstances trong vòng lặp lồng nhau (O(N*M)),
    // ta gom tất cả các pattern (tập hợp FeatureType) xuất hiện trong prevInstances vào một Set.
    // Việc này giúp kiểm tra sự tồn tại chỉ tốn O(log M) hoặc O(1).
    std::set<std::set<FeatureType>> validSubPatterns;
    for (const auto& prevInst : prevInstances) {
        std::set<FeatureType> pTypes;
        for (const auto& ptr : prevInst) {
            pTypes.insert(ptr->type);
        }
        validSubPatterns.insert(pTypes);
    }

    // 2. CHUẨN BỊ CHO ĐA LUỒNG
    // Lấy số lượng luồng tối đa
    int num_threads = omp_get_max_threads();
    // Tạo bộ đệm riêng cho từng luồng để tránh Race Condition khi push_back
    std::vector<std::vector<ColocationInstance>> thread_buffers(num_threads);

    // 3. XỬ LÝ SONG SONG
    #pragma omp parallel for
    for (size_t i = 0; i < instances.size(); ++i) {
        const auto& instance = instances[i];
        int thread_id = omp_get_thread_num();

        // Trích xuất feature types của instance hiện tại để so sánh
        std::set<FeatureType> instanceFeatures;
        for (const auto& instPtr : instance) {
            instanceFeatures.insert(instPtr->type);
        }

        for (const auto& candidate : candidates) {
            std::set<FeatureType> candidateSet(candidate.begin(), candidate.end());

            // Kiểm tra xem instance có chứa candidate pattern này không
            if (std::includes(instanceFeatures.begin(), instanceFeatures.end(),
                              candidateSet.begin(), candidateSet.end())) {
                
                bool allSubsetsExist = true;

                // Kiểm tra tất cả các tập con (k-1) của candidate
                for (size_t k = 0; k < candidate.size(); ++k) {
                    // Tạo subset bằng cách loại bỏ phần tử thứ k
                    std::set<FeatureType> subsetSet = candidateSet;
                    auto it = subsetSet.begin();
                    std::advance(it, k); // Di chuyển iterator đến vị trí k
                    subsetSet.erase(it);

                    // TRA CỨU NHANH: Kiểm tra subset có tồn tại trong dữ liệu cũ không
                    // Thay vì loop qua prevInstances, ta chỉ cần find trong set đã tạo ở bước 1
                    if (validSubPatterns.find(subsetSet) == validSubPatterns.end()) {
                        allSubsetsExist = false;
                        break;
                    }
                }

                if (allSubsetsExist) {
                    // Ghi vào buffer riêng của luồng hiện tại (An toàn)
                    thread_buffers[thread_id].push_back(instance);
                    break; // Đã tìm thấy candidate phù hợp, không cần check candidate khác cho instance này
                }
            }
        }
    }

    // 4. GỘP KẾT QUẢ (MERGE)
    // Gom dữ liệu từ các buffer con về vector kết quả chính
    std::vector<ColocationInstance> filteredInstances;
    for (const auto& buffer : thread_buffers) {
        filteredInstances.insert(filteredInstances.end(), buffer.begin(), buffer.end());
    }

    return filteredInstances;
}


std::vector<Colocation> JoinlessMiner::selectPrevColocations(
    const std::vector<Colocation>& candidates, 
    const std::vector<ColocationInstance>& instances, 
    double minPrev, 
    const std::map<FeatureType, int>& featureCount) 
{
    std::vector<Colocation> coarsePrevalent;

    // BƯỚC 1: Cấu trúc dữ liệu để gom nhóm (Aggregation)
    // Key: Candidate (Pattern)
    // Value: Map<FeatureType, Set<InstanceID>> - đếm số lượng instance unique cho từng feature
    std::map<Colocation, std::map<FeatureType, std::set<std::string>>> candidateStats;

    // Khởi tạo stats map cho các candidates (để đảm bảo candidate nào cũng có mặt dù không có instance)
    // Bước này tốn O(C), rất nhanh so với O(C*I)
    for (const auto& cand : candidates) {
        candidateStats[cand]; // Tạo entry rỗng
    }

    // BƯỚC 2: Duyệt Instance MỘT LẦN duy nhất (Single Pass)
    // Độ phức tạp: O(I * K * log(K)) với K là độ dài pattern (thường rất nhỏ)
    for (const ColocationInstance& instance : instances) {
        // 2a. Trích xuất Pattern từ Instance
        // Ví dụ: Instance có A1, B1 -> Pattern là {A, B}
        Colocation patternKey;
        for (const auto& instPtr : instance) {
            patternKey.push_back(instPtr->type);
        }
        // Đảm bảo patternKey được sort để khớp với key trong map (nếu candidate đã sort)
        // std::sort(patternKey.begin(), patternKey.end()); 

        // 2b. Kiểm tra xem pattern này có nằm trong danh sách candidates quan tâm không
        auto it = candidateStats.find(patternKey);
        if (it != candidateStats.end()) {
            // 2c. Cập nhật thống kê participating instances
            // it->second là map<Feature, Set<ID>>
            for (const auto& instPtr : instance) {
                it->second[instPtr->type].insert(instPtr->id);
            }
        }
    }

    // BƯỚC 3: Tính toán tỷ lệ và lọc (Duyệt qua Candidates)
    // Độ phức tạp: O(C * K)
    for (const auto& item : candidateStats) {
        const Colocation& candidate = item.first;
        const auto& participatingMap = item.second; // Map<Feature, Set<ID>>

        double min_participation_ratio = 1.0;
        bool possible = true;
        
        // Nếu không có instance nào tham gia -> participatingMap có thể thiếu feature hoặc set rỗng
        
        for (const auto& feature : candidate) {
            // Lấy tổng số lượng instance toàn cục của feature này
            auto totalIt = featureCount.find(feature);
            if (totalIt == featureCount.end() || totalIt->second == 0) {
                possible = false;
                break;
            }
            double totalFeatureCount = static_cast<double>(totalIt->second);

            // Lấy số lượng instance tham gia vào pattern
            int participatedCount = 0;
            auto partIt = participatingMap.find(feature);
            if (partIt != participatingMap.end()) {
                participatedCount = partIt->second.size();
            }

            double ratio = (double)participatedCount / totalFeatureCount;
            if (ratio < min_participation_ratio) {
                min_participation_ratio = ratio;
            }
        }

        if (possible && min_participation_ratio >= minPrev) {
            coarsePrevalent.push_back(candidate);
        }
    }

    return coarsePrevalent;
}