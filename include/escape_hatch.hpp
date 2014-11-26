#pragma once

namespace transducers {
    namespace details {
        template<typename _Boolean>
        class escape_hatch
        {
            _Boolean m_should_terminate = false;
        public:
            void request_termination()
            {
                m_should_terminate = true;
            }

            _Boolean should_terminate() const
            {
                return m_should_terminate;
            }
        };
    }

    class nonatomic_escape_hatch : public details::escape_hatch<bool>
    {
    };
}