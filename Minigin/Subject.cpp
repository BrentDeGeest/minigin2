#include "Subject.h"
#include "Observer.h"
#include "Event.h"

namespace dae
{
    void Subject::AddObserver(std::shared_ptr<Observer> observer)
    {
        m_Observers.push_back(observer);
    }

    void Subject::RemoveObserver(std::shared_ptr<Observer> observer)
    {
        m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
    }

    void Subject::Notify(std::shared_ptr<Event> event)
    {
        for (const auto& observer : m_Observers)
        {
            observer->OnNotify(event);
        }
    }
}
