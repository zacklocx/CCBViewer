
#ifndef RENDER_INCLUDED
#define RENDER_INCLUDED

#include <vector>
#include <memory>
#include <utility>

class render_obj_t
{
public:
	template <typename T>
	render_obj_t(T t) : self_(std::make_shared<model_t<T>>(std::move(t))) {}

private:
	struct concept_t
	{
		virtual ~concept_t() {}
		virtual void render_() const = 0;
	};

	template <typename T>
	struct model_t : concept_t
	{
		model_t(T t) : data_(std::move(t)) {}
		void render_() const { render(data_); }

		T data_;
	};

	std::shared_ptr<const concept_t> self_;

	friend void render(const render_obj_t& obj);
};

class render_cmd_t
{
public:
	int size() const { return (int)array_.size(); }

	const render_obj_t& operator[](int i) const { return array_[i]; }

	void add(const render_obj_t& obj) { array_.emplace_back(obj); }
	void clear() { array_.clear(); }

private:
	std::vector<render_obj_t> array_;
};

template <typename T>
void render(const T&) {}

inline void render(const render_obj_t& obj) { obj.self_->render_(); }
inline void render(const render_cmd_t& cmd) { int n = cmd.size(); for(int i = 0; i < n; ++i) render(cmd[i]); }

#endif /* RENDER_INCLUDED */
