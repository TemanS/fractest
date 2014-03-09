#ifndef FTNAMESPACE_H
#define FTNAMESPACE_H

namespace ft
{
    ///////////////////////////////////////////////////////////
    // Window Size
    ///////////////////////////////////////////////////////////
    const static int winWid = 360;
    const static int winHgt = 480;

    enum ft_tests {ft_lcm, ft_reduce, ft_combine, ft_tests_end};
    enum ft_level {ft_lvl_1, ft_lvl_2, ft_lvl_3, ft_lvl_end };
    enum ft_pb {ft_start, ft_stop, ft_defaults, ft_pb_end};
    enum ft_prob {ft_pnum, ft_problem, ft_answer, ft_prob_end};
    enum oplim {
        max_lt, max_num = max_lt,
        max_rt, max_den = max_rt,
        min_term, min_num = min_term,
        max_term, min_den = max_term,
        oplim_end};
}

#endif // FTNAMESPACE_H
