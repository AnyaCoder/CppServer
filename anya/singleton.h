#ifndef __ANYA_SINGLETON_H__
#define __ANYA_SINGLETON_H__

#include <memory>
namespace anya {
	template<class T, class X = void, int N = 0>
	class Singleton {
	public:
		static T* GetInstance() {
			static T v;
			return &v;
		}
	};

	template<class T, class X = void, int N = 0>
	class SingletonPtr {
	public:
		static std::shared_ptr<T> GetInstance() {
			static std::shared_ptr<T> v(new T);
			return v;
		}
	};

}
#endif