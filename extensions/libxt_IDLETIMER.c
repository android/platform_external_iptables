/*
 * Shared library add-on for iptables to add IDLETIMER support.
 *
 * Copyright (C) 2010 Nokia Corporation. All rights reserved.
 *
 * Contact: Luciano Coelho <luciano.coelho@nokia.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */
#include <stdio.h>
#include <xtables.h>
#include <linux/netfilter/xt_IDLETIMER.h>

enum {
	O_TIMEOUT = 0,
	O_LABEL,
	O_NETLINK,
        O_ALARM,
};

#define s struct idletimer_tg_info
static const struct xt_option_entry idletimer_tg_opts[] = {
	{.name = "timeout", .id = O_TIMEOUT, .type = XTTYPE_UINT32,
	 .flags = XTOPT_MAND | XTOPT_PUT, XTOPT_POINTER(s, timeout)},
	{.name = "label", .id = O_LABEL, .type = XTTYPE_STRING,
	 .flags = XTOPT_MAND | XTOPT_PUT, XTOPT_POINTER(s, label)},
	{.name = "send_nl_msg", .id = O_NETLINK, .type = XTTYPE_UINT8,
	 .flags = XTOPT_PUT, XTOPT_POINTER(s, send_nl_msg)},
	XTOPT_TABLEEND,
};
#undef s

#define s struct idletimer_tg_info_v1
static const struct xt_option_entry idletimer_tg_opts_v1[] = {
	{.name = "timeout", .id = O_TIMEOUT, .type = XTTYPE_UINT32,
	 .flags = XTOPT_MAND | XTOPT_PUT, XTOPT_POINTER(s, timeout)},
	{.name = "label", .id = O_LABEL, .type = XTTYPE_STRING,
	 .flags = XTOPT_MAND | XTOPT_PUT, XTOPT_POINTER(s, label)},
        {.name = "send_nl_msg", .id = O_NETLINK, .type = XTTYPE_UINT8,
	 .flags = XTOPT_PUT, XTOPT_POINTER(s, send_nl_msg)},
	{.name = "alarm", .id = O_ALARM, .type = XTTYPE_NONE},
	XTOPT_TABLEEND,
};
#undef s

static void idletimer_tg_help(void)
{
	printf(
"IDLETIMER target options:\n"
" --timeout time	Timeout until the notification is sent (in seconds)\n"
" --label string	Unique rule identifier\n"
" --send_nl_msg		(0/1) Enable netlink messages,"
			" and show remaining time in sysfs. Defaults to 0.\n"
"\n");
}

static void idletimer_tg_help_v1(void)
{
	printf(
"IDLETIMER target options:\n"
" --timeout time	Timeout until the notification is sent (in seconds)\n"
" --label string	Unique rule identifier\n"
" --send_nl_msg		(0/1) Enable netlink messages,"
			" and show remaining time in sysfs. Defaults to 0.\n"
" --alarm none	    Use alarm instead of default timer\n"
"\n");
}

static void idletimer_tg_print(const void *ip,
			       const struct xt_entry_target *target,
			       int numeric)
{
	struct idletimer_tg_info *info =
		(struct idletimer_tg_info *) target->data;

	printf(" timeout:%u", info->timeout);
	printf(" label:%s", info->label);
	printf(" send_nl_msg:%u", info->send_nl_msg);
}

static void idletimer_tg_print_v1(const void *ip,
			       const struct xt_entry_target *target,
			       int numeric)
{
	struct idletimer_tg_info_v1 *info =
		(struct idletimer_tg_info_v1 *) target->data;

	printf(" timeout:%u", info->timeout);
	printf(" label:%s", info->label);
        printf(" --send_nl_msg %u", info->send_nl_msg);
	if (info->timer_type == XT_IDLETIMER_ALARM)
		printf(" alarm");
}

static void idletimer_tg_save(const void *ip,
			      const struct xt_entry_target *target)
{
	struct idletimer_tg_info *info =
		(struct idletimer_tg_info *) target->data;

	printf(" --timeout %u", info->timeout);
	printf(" --label %s", info->label);
	printf(" --send_nl_msg %u", info->send_nl_msg);
}

static void idletimer_tg_save_v1(const void *ip,
			      const struct xt_entry_target *target)
{
	struct idletimer_tg_info_v1 *info =
		(struct idletimer_tg_info_v1 *) target->data;

	printf(" --timeout %u", info->timeout);
	printf(" --label %s", info->label);
        printf(" --send_nl_msg %u", info->send_nl_msg);
	if (info->timer_type == XT_IDLETIMER_ALARM)
		printf(" --alarm");
}

static void idletimer_tg_parse_v1(struct xt_option_call *cb)
{
	struct idletimer_tg_info_v1 *info = cb->data;

	xtables_option_parse(cb);
	if (cb->entry->id == O_ALARM)
		info->timer_type = XT_IDLETIMER_ALARM;
}

static struct xtables_target idletimer_tg_reg[] = {
         {
	        .family	       = NFPROTO_UNSPEC,
	        .name	       = "IDLETIMER",
	        .version       = XTABLES_VERSION,
	        .revision      = 0,
	        .size	       = XT_ALIGN(sizeof(struct idletimer_tg_info)),
	        .userspacesize = offsetof(struct idletimer_tg_info, timer),
	        .help	       = idletimer_tg_help,
	        .x6_parse      = xtables_option_parse,
	        .print	       = idletimer_tg_print,
	        .save	       = idletimer_tg_save,
	        .x6_options    = idletimer_tg_opts,
         },
         {
		.family	       = NFPROTO_UNSPEC,
		.name	       = "IDLETIMER",
		.version       = XTABLES_VERSION,
		.revision      = 1,
		.size	       = XT_ALIGN(sizeof(struct idletimer_tg_info_v1)),
		.userspacesize = offsetof(struct idletimer_tg_info_v1, timer),
		.help	       = idletimer_tg_help_v1,
		.x6_parse      = idletimer_tg_parse_v1,
		.print	       = idletimer_tg_print_v1,
		.save	       = idletimer_tg_save_v1,
		.x6_options    = idletimer_tg_opts_v1,
	},

};

void _init(void)
{
	xtables_register_targets(idletimer_tg_reg, ARRAY_SIZE(idletimer_tg_reg));
}
