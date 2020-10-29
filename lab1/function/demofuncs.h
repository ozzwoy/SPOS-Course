#pragma once

#include <chrono>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

namespace spos::lab1::demo {
	using namespace std::chrono_literals;
	using std::chrono::seconds;
	using std::pair;

	enum class op_group { AND, OR, INT, DOUBLE };

	std::string to_string(op_group group) {
        switch (group) {
            case op_group::AND:
                return "AND";
            case op_group::OR:
                return "OR";
            case op_group::INT:
                return "INT";
            default:
                return "DOUBLE";
        }
	}

	op_group get_group(std::string const &group_str) {
	    if (group_str == to_string(op_group::AND)) {
            return op_group::AND;
        } else if (group_str == to_string(op_group::OR)) {
            return op_group::OR;
	    } else if (group_str == to_string(op_group::INT)) {
	        return op_group::INT;
	    } else if (group_str == to_string(op_group::DOUBLE)) {
	        return op_group::DOUBLE;
	    }
        throw std::invalid_argument("Unknown operation group.");
	}

	template <typename T>
	struct func_info_holder {
		typedef T data_type;
		typedef std::pair<data_type, seconds> case_type;
		static const int CASES_NUM = 6;
		static constexpr seconds INF_TIME = 1000000000s;
	};

	template <op_group group>
	struct op_group_info_holder;

	template<>
	struct op_group_info_holder<op_group::AND> : public func_info_holder<bool> {
		static constexpr case_type f_cases[CASES_NUM] =
		        { pair{true, 5s}, pair{true, 10s}, pair{false, 10s},
                  pair{false, INF_TIME}, pair{true, 10s}, pair{false, INF_TIME} };;
		static constexpr case_type g_cases[CASES_NUM] =
		        { pair{true, 10s}, pair{true, 5s}, pair{false, INF_TIME},
                  pair{false, 10s}, pair{false, INF_TIME}, pair{true, 10s} };
		static constexpr case_type m_cases[CASES_NUM] =
		        { pair{false, 3s}, pair{true, 9s}, pair{true, 8s},
                  pair{false, 13s}, pair{true, INF_TIME}, pair{false, 10s} };
        static constexpr case_type n_cases[CASES_NUM] =
                { pair{true, 6s}, pair{true, 4s}, pair{true, 20s},
                  pair{true, 7s}, pair{true, 8s}, pair{false, 8s} };
		static constexpr bool ZERO = false;
	};


	template<>
	struct op_group_info_holder<op_group::OR> : public func_info_holder<bool> {
		static constexpr case_type f_cases[CASES_NUM] =
			{ pair{true, 5s}, pair{true, 10s}, pair{false, 10s},
              pair{false, INF_TIME}, pair{true, 10s}, pair{false, INF_TIME} };
		static constexpr case_type g_cases[CASES_NUM] =
			{ pair{true, 10s}, pair{true, 5s}, pair{false, INF_TIME},
              pair{false, 10s}, pair{false, INF_TIME}, pair{true, 10s} };
        static constexpr case_type m_cases[CASES_NUM] =
                { pair{false, 3s}, pair{true, 9s}, pair{true, 8s},
                  pair{false, 13s}, pair{true, INF_TIME}, pair{false, 10s} };
        static constexpr case_type n_cases[CASES_NUM] =
                { pair{true, 6s}, pair{true, 4s}, pair{true, 20s},
                  pair{true, 7s}, pair{true, 8s}, pair{false, 8s} };
        static constexpr bool ZERO = false;
	};

	template<>
	struct op_group_info_holder<op_group::INT> : public func_info_holder<int> {
		static constexpr case_type f_cases[CASES_NUM] =
			{ pair{7, 5s}, pair{7, 10s}, pair{0, 10s}, pair{0, INF_TIME},
              pair{7, 10s}, pair{0, INF_TIME} };
		static constexpr case_type g_cases[CASES_NUM] =
			{ pair{3, 10s}, pair{3, 5s}, pair{0, INF_TIME}, pair{0, 10s},
              pair{0, INF_TIME}, pair{3, 10s} };
        static constexpr case_type m_cases[CASES_NUM] =
                { pair{4, 3s}, pair{18, 9s}, pair{0, 8s},
                  pair{9, 13s}, pair{5, INF_TIME}, pair{0, 10s} };
        static constexpr case_type n_cases[CASES_NUM] =
                { pair{65, 6s}, pair{2, 4s}, pair{-5, 20s},
                  pair{16, 7s}, pair{13, 8s}, pair{0, 8s} };
        static constexpr int ZERO = 0;
	};

	template<>
	struct op_group_info_holder<op_group::DOUBLE> : public func_info_holder<double> {
		static constexpr case_type f_cases[CASES_NUM] =
			{ pair{7., 5s}, pair{7., 10s}, pair{0., 10s}, pair{0., INF_TIME},
              pair{7., 10s}, pair{0., INF_TIME} };
		static constexpr case_type g_cases[CASES_NUM] =
			{ pair{3., 10s}, pair{3., 5s}, pair{0., INF_TIME}, pair{0., 10s},
              pair{0., INF_TIME}, pair{3., 10s} };
        static constexpr case_type m_cases[CASES_NUM] =
                { pair{4., 3s}, pair{18., 9s}, pair{0., 8s},
                  pair{9., 13s}, pair{5., INF_TIME}, pair{0., 10s} };
        static constexpr case_type n_cases[CASES_NUM] =
                { pair{65., 6s}, pair{2., 4s}, pair{-5., 20s},
                  pair{16., 7s}, pair{13., 8s}, pair{0., 8s} };
        static constexpr double ZERO = 0.;
	};

	template <op_group group>
	typename op_group_info_holder<group>::data_type f_func(int x) {
		if (x < 0 || x >= func_info_holder<typename op_group_info_holder<group>::data_type>::CASES_NUM) {
			return typename op_group_info_holder<group>::data_type();
		}
		std::this_thread::sleep_for(op_group_info_holder<group>::f_cases[x].second);
		return op_group_info_holder<group>::f_cases[x].first;
	}

	template <op_group group>
	typename op_group_info_holder<group>::data_type g_func(int x) {
		if (x < 0 || x >= func_info_holder<typename op_group_info_holder<group>::data_type>::CASES_NUM) {
			return typename op_group_info_holder<group>::data_type();
		}
		std::this_thread::sleep_for(op_group_info_holder<group>::g_cases[x].second);
		return op_group_info_holder<group>::g_cases[x].first;
	}

    template <op_group group>
    typename op_group_info_holder<group>::data_type m_func(int x) {
        if (x < 0 || x >= func_info_holder<typename op_group_info_holder<group>::data_type>::CASES_NUM) {
            return typename op_group_info_holder<group>::data_type();
        }
        std::this_thread::sleep_for(op_group_info_holder<group>::m_cases[x].second);
        return op_group_info_holder<group>::m_cases[x].first;
    }

    template <op_group group>
    typename op_group_info_holder<group>::data_type n_func(int x) {
        if (x < 0 || x >= func_info_holder<typename op_group_info_holder<group>::data_type>::CASES_NUM) {
            return typename op_group_info_holder<group>::data_type();
        }
        std::this_thread::sleep_for(op_group_info_holder<group>::n_cases[x].second);
        return op_group_info_holder<group>::n_cases[x].first;
    }
}