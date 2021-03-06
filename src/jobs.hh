
#ifndef JOBS_HH
#define JOBS_HH

#include <string>
#include <cstring>
#include <vector>
#include <list>
#include <functional>

namespace WORKERS
{
class SUBTASK;
}


namespace JOBS
{

typedef std::string JOB_NAME;
typedef size_t PRIORITY;
typedef size_t TIME;
typedef size_t JOB_IDX;

class JOB_ENTRY;

namespace COST_CALC
{

typedef float COST;

COST get_cost_for_job(const JOB_ENTRY & job);

COST get_total_cost();

}

class JOB_STATUS
{
public:

	JOB_STATUS() = default;
	JOB_STATUS(const JOB_STATUS &) = delete;
	JOB_STATUS(JOB_STATUS &&) = default;
	JOB_STATUS & operator=(const JOB_STATUS &) = delete;
	JOB_STATUS & operator=(JOB_STATUS &&) = default;
	~JOB_STATUS() = default;

	//JOB_STATUS();

	const JOB_ENTRY & get_job() const;

	bool submitted() const;
	bool is_clean() const;
	TIME get_start_time() const;
	TIME get_complete_time() const;
	JOB_IDX get_parent() {return m_job_idx;}

	void set_parent(JOB_IDX idx);
	void reset();
	void add_subtask(const WORKERS::SUBTASK & subtask);

	std::string to_string() const
	{
		return
			std::to_string(m_job_idx) + " " +
			std::to_string(m_subtasks.size()) + " " +
			std::to_string(m_start_time) + " " +
			std::to_string(m_complete_time);
	}

private:
	std::vector<std::reference_wrapper<const WORKERS::SUBTASK>> m_subtasks;
	TIME m_start_time;
	TIME m_complete_time;
	JOB_IDX m_job_idx = 0;
	bool parent_set = false;
};


class JOB_ENTRY
{
public:

	JOB_ENTRY() = delete;
	JOB_ENTRY(const JOB_ENTRY &) = delete;
	JOB_ENTRY(JOB_ENTRY &&) = default;
	JOB_ENTRY & operator=(const JOB_ENTRY &) = delete;
	JOB_ENTRY & operator=(JOB_ENTRY &&) = default;
	~JOB_ENTRY() = default;

	JOB_ENTRY(JOB_NAME && name, PRIORITY pri, size_t num_subtasks,
		TIME earliest_start_time, TIME subtask_duration);
	void set_idx(JOB_IDX idx);

	const JOB_STATUS & get_status() const;
	JOB_STATUS & get_modifiable_status();
	const JOB_NAME & get_name() const;
	PRIORITY get_priority() const;
	size_t get_num_subtasks() const;
	TIME get_earliest_start_time() const;
	TIME get_subtask_duration() const;
	JOB_IDX get_index() const {return m_idx;}

	std::string to_string() const;

private:
	JOB_STATUS m_status;
	JOB_NAME m_name;
	PRIORITY m_priority;
	size_t m_num_subtasks;
	TIME m_earliest_start_time;
	TIME m_subtask_duration;
	JOB_IDX m_idx = 0;
	bool m_id_set = false;
};


class JOB_QUEUE
{
private:
	typedef std::reference_wrapper<JOB_ENTRY> JOB_Q_ENTRY;
	typedef std::list<JOB_Q_ENTRY> CONTAINER;
public:
	typedef CONTAINER::iterator ITER;
	typedef CONTAINER::const_iterator CITER;

	JOB_QUEUE & operator=(const JOB_QUEUE &) = delete;
	JOB_QUEUE & operator=(JOB_QUEUE &&) = delete;

	void erase(ITER job_iter);

	ITER begin();
	ITER end();

	CITER cbegin() const;
	CITER cend() const;

	bool empty() const;
	size_t size() const;

	friend std::ostream & operator<<(std::ostream & os, const JOB_QUEUE & job_q);

	static void load();
	static JOB_QUEUE & get_inst();

private:
	JOB_QUEUE();
	JOB_QUEUE(const JOB_QUEUE &) = delete;
	JOB_QUEUE(JOB_QUEUE &&) = delete;
	~JOB_QUEUE() = default;

	void add_job(JOB_ENTRY & job);

	CONTAINER m_jobs;

	static JOB_QUEUE * m_job_queue_inst;
};

class JOB_POOL
{
private:
	typedef std::vector<JOB_ENTRY> CONTAINER;
	typedef CONTAINER::iterator ITER;
public:
	typedef CONTAINER::const_iterator CITER;

	JOB_POOL & operator=(const JOB_POOL &) = delete;
	JOB_POOL & operator=(JOB_POOL &&) = delete;

	// Modifiers
	void add_job(JOB_ENTRY && job);
	void sort_and_create_index();

	// Accessors
	bool empty() const;
	size_t size() const;
	bool is_ready() const;

	ITER begin();
	ITER end();
	CITER cbegin() const;
	CITER cend() const;

	JOB_ENTRY & operator[](size_t idx) { return m_jobs[idx]; }

	friend std::ostream & operator<<(std::ostream & os, const JOB_POOL & job_q);

	static JOB_POOL & get_inst();

private:
	JOB_POOL() = default;
	JOB_POOL(const JOB_POOL &) = delete;
	JOB_POOL(JOB_POOL &&) = delete;
	~JOB_POOL() = default;

	void re_index();

	CONTAINER m_jobs;
	bool m_sorted_and_indexed = false;

	static JOB_POOL * m_instance;
};

std::ostream & operator<<(std::ostream & os, const JOB_QUEUE & job_q);
std::ostream & operator<<(std::ostream & os, const JOB_POOL & job_q);

}



#endif
