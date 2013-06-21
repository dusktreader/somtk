#! /usr/bin/env python

import os, sys
from argparse import ArgumentParser

def main():

    parser = ArgumentParser( description="Create image library markup files",
                             usage="usage: %(prog)s [ options ] ..." )

    parser.add_argument( '-i', '--image_dir',
                         action='store', default='.',
                         help="Set the source folder that holds the library images to DIR", metavar='DIR' )

    parser.add_argument( '-o', '--datafile_dir',
                         action='store', default='<default>',
                         help="Set the destination folder for the generated library data files to DIR", metavar='DIR' )

    parser.add_argument( '-n', '--library_name',
                         action='store', default='generic',
                         help="Set the name of the library to generate to NAME", metavar='NAME' )

    parser.add_argument( '-b', '--force_balance',
                         action='store_true',
                         help="Force the library to equalize number of items in each category (the minimum)" )

    parser.add_argument( '-a', '--force_abspaths',
                         action='store_true',
                         help="Force the library data files to use absolute paths" )

    parser.add_argument( '-s', '--split_ratio',
                         action='store', default=0.9, type=float,
                         help="Set the ratio of training instances to testing instances to NUM", metavar='NUM' )

    parser.add_argument( '-l', '--training_limit',
                         action='store', default=sys.maxint, type=int,
                         help="Set a limit for the maximum number of training instances for each category to NUM", metavar='NUM' )

    parser.add_argument( '-v', '--verbose',
                         action='store_true',
                         help="Print too much information" )
    
    args = parser.parse_args()
    
    if args.split_ratio <= 0.0 or args.split_ratio >= 1.0:
        parser.error("The split ratio must be greater than 0.0 and less than 1.0")

    if args.training_limit < 0:
        parser.error("The training limit may not be less than 0")
    
    image_dir = os.path.normpath( args.image_dir )
    datafile_dir = ''
    if args.datafile_dir == '<default>':
        datafile_dir = image_dir
    else:
        datafile_dir = os.path.normpath( args.datafile_dir )
    
    if args.verbose:
        print >> sys.stderr, "Looking for image category subforlders in {image_dir}".format( image_dir=image_dir )
           
    category_dict = {}
    
    category_list = [ c for c in os.listdir( image_dir ) if os.path.isdir( image_dir + os.sep + c ) ]

    category_count = len( category_list )    
    
    if args.verbose:
        print >> sys.stderr, "Found {category_count} categories: {category_list}".format( category_count=category_count, category_list=category_list )
        
    min_count = sys.maxint

    for category in category_list:
    
        subdir = image_dir + os.sep + category    
    
        if args.verbose:
            print >> sys.stderr, "Looking for category items in {subdir}".format( subdir=os.path.abspath( subdir ) )
        
        # TODO:  add extension filter
        
        category_dict[ category ] = os.listdir( subdir )
        item_count = len( category_dict[ category ] )
        
        if args.verbose:
            print >> sys.stderr, "Found {item_count} category items".format( item_count=item_count )
        
        min_count = min( min_count, item_count )

    if args.force_balance:
        args.training_limit = min(args.training_limit, min_count)

    if args.training_limit < sys.maxint:
        if args.verbose:
            print >> sys.stderr, "Truncating item list for each category to {training_limit} (minimum)".format(training_limit=args.training_limit)
        for category in category_list:
            category_dict[category] = category_dict[category][:args.training_limit]
    
    if args.verbose:
        print >> sys.stderr, "Creating output library files in {datafile_dir}".format( datafile_dir = datafile_dir )
        
        path_type = 'absolute' if args.force_abspaths else 'relative'
        print >> sys.stderr, "Filenames in datafiles will be written as {path_type} paths".format( path_type = path_type )
        
    training = open( datafile_dir + os.sep + 'training_library.xml', 'w' )
    testing = open( datafile_dir + os.sep + 'testing_library.xml', 'w' )
        
    print >> training, '<?xml version="1.0"?>'
    print >> testing, '<?xml version="1.0"?>'
    
    print >> training, '<library_document>'
    print >> testing, '<library_document>'
    
    
    print >> training, '  <library name="{library_name}_training" categories="{category_count}">'.format( library_name=args.library_name, category_count=category_count )
    print >> testing, '  <library name="{library_name}_testing" categories="{category_count}">'.format( library_name=args.library_name, category_count=len( category_dict ) )
    
    category_id = 0
    for category_name in category_list:
        
        category_items = category_dict[ category_name ]
        category_size = len( category_items )
        splitter = int( category_size * args.split_ratio )
        training_size = splitter
        testing_size = category_size - splitter
        
        if args.verbose:
            print >> sys.stderr, "Category {category_name} will be split with {training_size} training instances and {testing_size} testing instances".format( category_name=category_name, training_size=training_size, testing_size=testing_size )
        
        print >> training, '    <category id="{category_id}" name="{category_name} count={category_size}">'.format( category_id=category_id, category_name=category_name, category_size=training_size )
        print >> testing, '    <category id="{category_id}" name="{category_name} count={category_size}">'.format( category_id=category_id, category_name=category_name, category_size=testing_size )
        
        for category_item in category_items[ : splitter ]:
            image_path = image_dir + os.sep + category_name + os.sep + category_item
            if args.force_abspaths:
                image_path = os.path.abspath( image_path )
            print >> training, '      <image path="{image_path}" />'.format( image_path=image_path )
        
        for category_item in category_items[ splitter: ]:
            image_path = image_dir + os.sep + category_name + os.sep + category_item
            if args.force_abspaths:
                image_path = os.path.abspath( image_path )
            print >> testing, '      <image path="{image_path}" />'.format( image_path=image_path )     
            
        print >> training, '    </category>'
        print >> testing, '    </category>'
        
        category_id += 1
        
    
    print >> training, '  </library>'
    print >> testing, '  </library>'
    
    print >> training, '</library_document>'
    print >> testing, '</library_document>'
    
    training.close()
    testing.close()
        
        

if __name__ == '__main__':
    main()
