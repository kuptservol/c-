#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
    NEW,          // новая
    IN_PROGRESS,  // в разработке
    TESTING,      // на тестировании
    DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
public:
    // Получить статистику по статусам задач конкретного разработчика
    const TasksInfo &GetPersonTasksInfo(const string &person) const {
        return user_to_task_info.at(person);
    }

    // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
    void AddNewTask(const string &person) {
        user_to_task_info[person][TaskStatus::NEW]++;
    }

    // Обновить статусы по данному количеству задач конкретного разработчика,
    // подробности см. ниже
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count) {
        TasksInfo &users_tasks = user_to_task_info[person];

        TasksInfo performed_tasks;
        TasksInfo non_performed_tasks;

        int tasks = users_tasks.size();

        vector<TaskStatus> users_tasks_list;

        for (const auto &[key, value] : users_tasks) {
            for (int i = value; i >= 1; i--) {
                users_tasks_list.push_back(key);
            }
        }

        for (const auto key : users_tasks_list) {
            if (task_count >= 1) {
                if (key == TaskStatus::NEW) {
                    users_tasks[TaskStatus::NEW]--;
                    users_tasks[TaskStatus::IN_PROGRESS]++;
                    performed_tasks[TaskStatus::IN_PROGRESS]++;
                } else if (key == TaskStatus::IN_PROGRESS) {
                    users_tasks[TaskStatus::IN_PROGRESS]--;
                    users_tasks[TaskStatus::TESTING]++;
                    performed_tasks[TaskStatus::TESTING]++;
                } else if (key == TaskStatus::TESTING) {
                    users_tasks[TaskStatus::TESTING]--;
                    users_tasks[TaskStatus::DONE]++;
                    performed_tasks[TaskStatus::DONE]++;
                }
            } else {
                if (key == TaskStatus::NEW) {
                    non_performed_tasks[TaskStatus::NEW]++;
                } else if (key == TaskStatus::IN_PROGRESS) {
                    non_performed_tasks[TaskStatus::IN_PROGRESS]++;
                } else if (key == TaskStatus::TESTING) {
                    non_performed_tasks[TaskStatus::TESTING]++;
                }
            }

            task_count--;
        }

        vector<TaskStatus> empty;
        for (const auto &[key, value] : users_tasks) {
            if (value == 0) {
                empty.push_back(key);
            }
        }

        for (auto e : empty) {
            users_tasks.erase(e);
        }

        return {performed_tasks, non_performed_tasks};
    }

private:
    map<string, TasksInfo> user_to_task_info;
};

void PrintTasksInfo(TasksInfo tasks_info) {
    cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
         ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
         ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
         ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

int main() {
    TeamTasks tasks;
    tasks.AddNewTask("Ilia");
    for (int i = 0; i < 3; ++i) {
        tasks.AddNewTask("Ivan");
    }
    cout << "Ilia's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
    cout << "Ivan's tasks: ";
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) =
            tasks.PerformPersonTasks("Ivan", 2);
    cout << "Updated Ivan's tasks: ";
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: ";
    PrintTasksInfo(untouched_tasks);

    return 0;
}

/*
Ilia's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Ivan's tasks: 3 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 2 tasks in progress, 0 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 1 new tasks, 0 tasks in progress, 0 tasks are being tested, 0 tasks are done
Updated Ivan's tasks: 0 new tasks, 1 tasks in progress, 1 tasks are being tested, 0 tasks are done
Untouched Ivan's tasks: 0 new tasks, 1 tasks in progress, 0 tasks are being tested, 0 tasks are done
 */