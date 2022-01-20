
import os

build_info_file_name = 'buildInfo.h'
build_info_path = '/Users/callwyat/Development/Quiet/src/Quiet.X/' + build_info_file_name

statusStream = os.popen('git status')
statusLines = str.join('\n', statusStream.readlines())

clean_tree = '' if 'Clean' in statusLines else '~'

with open(build_info_path, 'w') as f:

    f.write('#ifndef ANALOGINPUTS_H\n')
    f.write('#define	ANALOGINPUTS_H\n')
    f.write('\n')
    f.write('#ifdef	__cplusplus\n')
    f.write('extern "C" {\n')
    f.write('#endif\n')
    f.write('\n')

    logStream = os.popen('git log --pretty=format:"%H\n%an\n%ad" -n 1')
    f.write('#define COMMIT_HASH "%s%s%s"\n' % (clean_tree, logStream.readline().strip(), clean_tree))
    f.write('#define COMMIT_AUTHOR "%s"\n' % logStream.readline().strip())
    f.write('#define COMMIT_DATE "%s"\n' % logStream.readline().strip())

    f.write('\n')
    f.write('#ifdef	__cplusplus\n')
    f.write('}\n')
    f.write('#endif\n')
    f.write('\n')
    f.write('\n')
    f.write('#endif	/* ANALOGINPUTS_H */\n')
    f.write('\n')

if clean_tree != '':
    print('Warning: "%s" build on an unclean working tree' % build_info_file_name)
