#pragma once
#include <memory>

template <class Val>
class LinkedList
{
protected:
	std::shared_ptr<Node<Val>> head_, tail_;
public:
	LinkedList() {};
	~LinkedList() 
	{
		std::shared_ptr<Node<Val>> curr = head_;
		head_.reset();
		tail_.reset();

		while (curr)
		{
			auto temp = curr->next_;
			curr.reset();
			curr = temp;
		}
	};

	template <class NodeVal, std::enable_if_t<std::is_base_of_v<Val, NodeVal>>, int> = 0>
	class Node
	{
		friend class LinkedList;
	protected:
		std::shared_ptr<Node<NodeVal>> next_, prev_;
		std::shared_ptr<NodeVal> val_;
	public:
		template <class ... Args, std::enable_if_t<std::is_constructible<NodeVal, Args&&...>, int> = 0>
		Node(Args&&... args, std::shared_ptr<Node<NodeVal>> prev, std::shared_ptr<Node<NodeVal>> next)
			: val_(std::make_shared<NodeVal>(std::forward(args))), prev_(prev), next_(next)
		{}

		inline std::shared_ptr<NodeVal> GetValue() const { return val_; }

		~Node();
	};

	inline void AddNode(std::shared_ptr<Node<Val>> node)
	{
		tail_->next_ = node;
		tail_ = tail->next;
	}

	void RemoveNode(std::shared_ptr<Node<Val>> node)
	{
		if (!node->prev_) // It has no prev node (head)
			head_ = node->next_;
		else
			node->prev_ = node->next_;
	}
};

template<class Val>
inline LinkedList<Val>::~Node()
{
	next_.reset();
	prev_.reset();
	val_.reset();
}
