
#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#include <vector>
#include <memory>
#include <utility>

template<typename T>
void render(const T&) {}

class render_obj_t
{
public:
	template<typename T>
	render_obj_t(T t)
		: self_(std::make_shared<model_t<T>>(std::move(t)))
	{}

private:
	struct concept_t
	{
		virtual ~concept_t() {}
		virtual void render_() const = 0;
	};

	template<typename T>
	struct model_t : concept_t
	{
		model_t(T t) : data_(std::move(t)) {}
		void render_() const { render(data_); }

		T data_;
	};

	std::shared_ptr<const concept_t> self_;

	friend void render(const render_obj_t& obj) { obj.self_->render_(); }
};

using render_cmd_t = std::vector<render_obj_t>;

inline void render(const render_cmd_t& cmd) { for(const auto& obj : cmd) render(obj); }

#endif /* RENDER_INCLUDED */
